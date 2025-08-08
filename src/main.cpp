#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "data.h"

WiFiMulti wifiMulti;
WebServer server(80);

void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file || file.isDirectory()) {
    server.send(500, "text/plain", "No se pudo abrir index.html");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void setup() {
  Serial.begin(921600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (!LittleFS.begin(true)) {
    Serial.println("Error al montar LittleFS incluso tras formatear.");
    return;
  }

 
  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
  wifiMulti.addAP(ssid3, password3);

  Serial.println("Conectando a WiFi...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  server.on("/", handleRoot);
  server.serveStatic("/", LittleFS, "/");
  server.begin();
  Serial.println("Servidor HTTP iniciado");

   server.on("/favicon.ico", HTTP_GET, [](){
    server.send(204); 
  });
}

void loop() {
  server.handleClient();
}
