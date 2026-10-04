// src/http/http.cpp

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "../indication/indication.h"
#include "../monitor/monitor.h"
#include "../project_config.h"
#include "../serialization/serialization.h"
#include "http.h"

// ---------------------------------
#define SERVER_URL "http://httpbun.com/post" // HTTP POST

// ---------------------------------

static uint8_t httpMessageSequence = 0;
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

  if (!serializeTelemetry(telemetry,

                          httpMessageSequence, payload, sizeof(payload))) {

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
    httpMessageSequence++;
    Serial.println(
        "[HTTP] Server response: '200' (The server has received the data)");
    Serial.println("------------");
  } else {
    Serial.print("[HTTP] Error: ");
    Serial.println(httpCode);
  }

  http.end();
}
