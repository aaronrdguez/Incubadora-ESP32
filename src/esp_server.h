#ifndef ESP_SERVER_H
#define ESP_SERVER_H

#include <WiFi.h>
#include <WebServer.h>
//#include <ESPmDNS.h>

#include "handlers.h"

IPAddress local_ip(192, 168, 1, 100);  // Esta será la IP para entrar a la web
IPAddress gateway(192, 168, 1, 1);    // Puerta de enlace en la misma subred (192.168.1.x)
IPAddress subnet(255, 255, 255, 0);   // Máscara de subred estándar

extern WebServer server;

const char* ssid = "Incubadora_C6_Final";  // Tu SSID
const char* password = "12345678";  //Tu Clave

void setupServer() {
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  //MDNS.begin("incubadora");
  //MDNS.addService("http", "tcp", 80);

  server.on("/", HTTP_GET, handleOn);
  server.on("/update", HTTP_POST, handleUpdate);
  server.begin();
}

#endif