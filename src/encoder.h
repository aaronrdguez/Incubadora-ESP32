#ifndef ENCODER_H
#define ENCONDER

#include <ESP32Encoder.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// --- NUEVOS PINES ENCODER (GP2, GP3, GP15) ---
#define ENC_CLK 2
#define ENC_DT  3
#define ENC_SW  15

int modoMenu = 0; 
long ultimaPos = 0;

extern float minTemp, maxTemp;
extern int minHum, maxHum, rotIntervalo, rotDuracion;
extern String fechaInicio, fechaFin;

extern ESP32Encoder enconder;
extern LiquidCrystal_I2C lcd;

void setupEncoder() {
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachHalfQuad(ENC_CLK, ENC_DT);
  encoder.setCount(0);
}

void sendMenu(String title, float value, String prefix) {
  lcd.setCursor(0, 0);
  lcd.print("EDITAR " + title);
  lcd.setCursor(0, 1);
  lcd.print("> " + String(value, 1) + " " + prefix);
}

void sendError(String context) {
  lcd.setCursor(0, 0);
  lcd.print("Error in: " + context);
}

void loopEncoder(float now) {
  long currentPos = encoder.getCount();
  
  static bool ultimoEstadoBtn = HIGH;

  bool lecturaBtn = digitalRead(ENC_SW);
  if (lecturaBtn == LOW && ultimoEstadoBtn == HIGH) {
    modoMenu++;
    if (modoMenu > 4) modoMenu = 0;
    encoder.setCount(0); 
    lcd.clear();
    delay(200); // Debounce simple
  }
  ultimoEstadoBtn = lecturaBtn;

  // Ajuste de variables desde el encoder
  if (currentPos != ultimaPos) {
    float diff = (currentPos - ultimaPos) * 0.1; 
    if (modoMenu == 1) minTemp += diff; 
    if (modoMenu == 2) maxTemp += diff; 
    if (modoMenu == 3) minHum += (int)(diff * 10); 
    if (modoMenu == 4) maxHum += (int)(diff * 10);
    ultimaPos = currentPos;
  }

  static unsigned long lastLcdUpdate = 0;
  if (now - lastLcdUpdate >= 500) {
    lastLcdUpdate = now;
    
    switch(modoMenu) {
      case 0:
      lcd.setCursor(0, 0);
      lcd.print("T:" + String(temp, 1) + " H:" + String(hum, 0) + "% ");
      lcd.setCursor(0, 1);
      lcd.print(String(minTemp, 1) + "-" + String(maxTemp, 1) + " C ");
      break;
      case 1: sendMenu("T. MIN", minTemp, " °C"); break;
      case 2: sendMenu("T. MAX", maxTemp, " °C"); break;
      case 3: sendMenu("H. MIN", minHum, " %"); break;
      case 4: sendMenu("H. MAX", maxHum, " %"); break;
    }
  }
}
#endif