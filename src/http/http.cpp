// src/http/http.cpp

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "../config.h"
#include "../indication/indication.h"
#include "../monitor/monitor.h"
#include "../serialization/serialization.h"
#include "http.h"

// ---------------------------------
static void sendData(const Telemetry &telemetry);

// ---------------------------------
void handleSendData(Telemetry &telemetry) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HTTP] Wi-Fi does not connect");
    return;
  }

  sendData(telemetry);
}

static void sendData(const Telemetry &telemetry) {
  HTTPClient http;
  http.begin(SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  char payload[512];

  if (!serializeTelemetry(telemetry, payload, sizeof(payload))) {

    Serial.println("[HTTP] Failed to serialize telemetry");
    http.end();
    return;
  }

  Serial.print("[HTTP] Sending: ");

#if defined(DEBUG_MODE)
  printMonitorPayload(payload);
#endif

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
