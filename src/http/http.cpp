// src/http/http.cpp

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "../config.h"
#include "../monitor/monitor.h"
#include "../serialization/serialization.h"
#include "http.h"

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
#include "../serialization/serializers/telemetry_serializer_aj.h"
#else
#include "../serialization/serializers/telemetry_serializer.h"
#endif

// ---------------------------------

// static uint8_t sequenceCounter = 0;
static void sendData(const Telemetry &telemetryData);

// ---------------------------------

void handleSendData(Telemetry &telemetryData) {
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

  if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {

    Serial.println("[HTTP] Failed to serialize telemetry");
    http.end();
    return;
  }

  Serial.print("[HTTP] Sending: ");
  printMonitorPayload(payload);

  int httpCode =
      http.POST(reinterpret_cast<uint8_t *>(payload), strlen(payload));

  if (httpCode == 200) {
    Serial.println(
        "[HTTP] Server response: '200' (The server has received the data)");
    Serial.println("------------");
  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(httpCode);
  }

  http.end();
}
