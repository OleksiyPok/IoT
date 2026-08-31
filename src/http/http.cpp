// src/http/http.cpp

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "../config.h"
#include "http.h"

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
#include "../telemetry/telemetry_serializer_aj.h"
#else
#include "../telemetry/telemetry_serializer.h"
#endif

// ---------------------------------

static void sendData(const Telemetry &telemetryData);

// ---------------------------------

void handleSendData(const Telemetry &telemetryData) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] Wi-Fi does not connect");
    return;
  }

  sendData(telemetryData);
}

static void sendData(const Telemetry &telemetryData) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  char payload[256];

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
  if (!serializeTelemetryArduinoJson(telemetryData, payload, sizeof(payload))) {
#else
  if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {
// #error "No telemetry serializer selected"
#endif

    Serial.println("[HTTP] Failed to serialize telemetry");
    http.end();
    return;
  }

  Serial.print("[HTTP] Sending: ");
  Serial.println(payload);

  int httpCode =
      http.POST(reinterpret_cast<uint8_t *>(payload), strlen(payload));

  if (httpCode == 200) {
    Serial.println(
        "[HTTP] Server response: '200' (The server has received the data)");
  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(httpCode);
  }

  http.end();
}