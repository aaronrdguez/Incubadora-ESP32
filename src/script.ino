#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP32Encoder.h>

// --- TUS PINES DEFINIDOS ---
#define PIN_DHT 22
#define PIN_FOCOS 21
#define PIN_MOTOR 20
#define PIN_FAN 19
#define PIN_BUZZER 4

// --- OBJETOS ---
DHT dht(PIN_DHT, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP32Encoder encoder;
WebServer server(80);

// --- VARIABLES DE CONTROL ---
float minTemp = 37.2;
float maxTemp = 37.8;
int minHum = 50;
int maxHum = 60;
int rotIntervalo = 2;
int rotDuracion = 15;

bool isIncubating = false;
bool error = false;
unsigned long tiempoInicioMillis = 0;

String fechaFin = "2023-10-22T12:00";

float temp, hum;


#include "handlers.h"
#include "esp_server.h"
#include "encoder.h"

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_FOCOS, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(ENC_SW, INPUT_PULLUP);

  // Bus I2C (SDA:0, SCL:1)
  Wire.begin(0, 1); 
  dht.begin();

  lcd.init();
  lcd.backlight();

  setupEncoder();
  setupServer();
}

float getTemp() {
  return dht.readTemperature();
}

float getHum() {
  return dht.readHumidity();
}

bool checkError() {
  if (isnan(temp) || isnan(hum)) {
    lcd.clear();
    sendError("SENSOR DHT");
    
    digitalWrite(PIN_FOCOS, LOW);
    digitalWrite(PIN_FAN, LOW);
    digitalWrite(PIN_MOTOR, LOW);
    return true;
  }

  if (isIncubating) {
    if (temp < (minTemp - 5.0) || temp > (maxTemp + 5.0)) {
      lcd.clear();
      sendError("TEMP CRITICA");
      return true;
    }
    if (hum < 10 || hum > 90) {
      lcd.clear();
      sendError("HUMEDAD LIM");
      return true;
    }
  }

  return false;
}

void loop() {
  server.handleClient();

  unsigned long now = millis();

  // Lectura de Temperatura y Humedad
  static unsigned long lastSensorRead = 0;
  if (now - lastSensorRead >= 2000) {
    lastSensorRead = now;

    temp = getTemp();
    hum = getHum();
  }
  
  if (checkError()) {
    error = checkError(); 
    return;
  }
  
  if (error) {
    lcd.clear();
    error = false;
  }
  
  loopEncoder(now);

  if (!isIncubating) return; 

  // Control de Focos (Calefacción) 
  if (temp < minTemp) {
    digitalWrite(PIN_FOCOS, HIGH); 
  } else if (temp > maxTemp) {
    digitalWrite(PIN_FOCOS, LOW); 
  }

  // Control de Ventilador (Humedad)
  if (hum > maxHum) {
    digitalWrite(PIN_FAN, HIGH);   
  } else if (hum < minHum) {
    digitalWrite(PIN_FAN, LOW);
  }

  // --- 4. CONTROL DE ROTACIÓN (MOTOR/SERVO) ---
  static unsigned long ultimaRotacion = 0;
  static bool motorEncendido = false;
  
  // Convertimos el intervalo de horas a milisegundos
  unsigned long intervaloMs = (unsigned long)rotIntervalo * 3600000;

  // Si no está rotando y pasó el tiempo de intervalo
  if (!motorEncendido && (now - ultimaRotacion >= intervaloMs)) {
    motorEncendido = true;
    ultimaRotacion = now;
    digitalWrite(PIN_MOTOR, HIGH);
    Serial.println(">>> ROTACIÓN INICIADA");
  }

  // Si el motor está encendido, verificar si ya cumplió su duración
  if (motorEncendido && (now - ultimaRotacion >= (unsigned long)rotDuracion * 1000)) {
    motorEncendido = false;
    // No reseteamos ultimaRotacion aquí para que el intervalo cuente desde el inicio del giro
    digitalWrite(PIN_MOTOR, LOW);
    Serial.println(">>> ROTACIÓN FINALIZADA");
  }
}