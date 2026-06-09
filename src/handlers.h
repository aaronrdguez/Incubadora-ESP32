#ifndef HANDLERS_H
#define HANDLERS_H

#include <ArduinoJson.h>
#include <WebServer.h>

#include "web_interface.h"

extern WebServer server;

extern float minTemp, maxTemp;
extern int minHum, maxHum, rotIntervalo, rotDuracion;
extern String fechaInicio, fechaFin;

void handleUpdate() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "No hay datos");
    return;
  }

  String body = server.arg("plain");
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "text/plain", "JSON Inválido");
    return;
  }

  String cmd = doc["cmd"];

  if (cmd == "start") {
    isIncubating = true;
    tiempoInicioMillis = doc["ts"].as<unsigned long>(); 
    Serial.println("Incubación iniciada");
  } 
  else if (cmd == "stop") {
    isIncubating = false;
    tiempoInicioMillis = 0;
    Serial.println("Incubación detenida");
  } 
  else if (cmd == "save") {
    fechaFin = doc["fecha_fin"].as<String>();
    minTemp = doc["t_mi"].as<float>();
    maxTemp = doc["t_ma"].as<float>();
    minHum = doc["h_mi"].as<int>();
    maxHum = doc["h_ma"].as<int>();
    rotIntervalo = doc["r_i"].as<int>();
    rotDuracion = doc["r_d"].as<int>();
    Serial.println("Configuración actualizada");
  }

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleOn() {
  String html = index_html;
  
  // Reemplazo de etiquetas dinámicas para la web
  html.replace("%FECHA_FIN%",     fechaFin);
  html.replace("%T_MIN%",         String(minTemp, 1));
  html.replace("%T_MAX%",         String(maxTemp, 1));
  html.replace("%H_MIN%",         String(minHum));
  html.replace("%H_MAX%",         String(maxHum));
  html.replace("%R_INT%",         String(rotIntervalo));
  html.replace("%R_DUR%",         String(rotDuracion));
  html.replace("%TEMP_ACTUAL%",   String(temp, 1));
  html.replace("%HUM_ACTUAL%",    String(hum, 0));
  html.replace("%START_TIME_VAL%", String(tiempoInicioMillis));

  server.send(200, "text/html", html);
}

#endif