// src/http/http.cpp

#include "http.h"
#include "../config.h"
#include "../wifi/wifi.h"
#include <Arduino.h>
#include <HTTPClient.h>

// ---------------------------------
void handleSendData(Telemetry &telemetryData);
void sendData(const float &temperature, const float &humidity,
              const float &lux);
// ---------------------------------

void handleSendData(Telemetry &telemetryData) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] Wi-Fi does not connected");
    return;
  }

  sendData(telemetryData.dht.temperature, telemetryData.dht.humidity,
           telemetryData.ldr.lux);
};

void sendData(const float &temperature, const float &humidity,
              const float &lux) {

  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  char payload[128];
  snprintf(payload, sizeof(payload),
           "{\"temperature\":%.1f,\"humidity\":%.1f,\"lux\":%.1f}", temperature,
           humidity, lux);

  Serial.print("[HTTP] Sending: ");
  Serial.println(payload);

  int httpCode = http.POST((uint8_t *)payload, strlen(payload));

  if (httpCode == 200) {
    Serial.println(
        "[HTTP] Server response: '200' (The server has received the data)");
  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(httpCode);
  }

  http.end();
};