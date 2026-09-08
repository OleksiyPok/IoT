// src/monitor/monitor.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../config.h"
#include "../telemetry/telemetry.h"
#include "../time/time.h"
#include "monitor.h"

// ---------------------------------

static void printJsonPretty(const JsonDocument &doc);

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
}

void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &systemState) {

#if defined(DEBUG_MODE)

  Serial.println("================================");
  Serial.println("| !!!!!!!! DEBUG MODE !!!!!!!! |");
  Serial.println("|  Comment out ''DEBUG_MODE''  |");
  Serial.println("|    in the configuration      |");
  Serial.println("| to switch to Production mode |");
  Serial.println("================================");

#endif
}

void printMonitorPayload(const char *payload) {

  if (payload == nullptr) {
    Serial.println("Payload is null");
    return;
  }

  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("JSON parse error: ");
    Serial.println(error.c_str());
    return;
  }

  Serial.println("[MQTT] JSON:");

  printJsonPretty(doc);

  // Serial.println("------------");
}

static void printJsonPretty(const JsonDocument &doc) {
  serializeJsonPretty(doc, Serial);
  Serial.println();
}