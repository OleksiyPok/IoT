// src/monitor/monitor.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../config.h"
#include "../system/system_state.h"
#include "../time/time.h"
#include "monitor.h"

// ---------------------------------
static void printJsonPretty(const JsonDocument &doc);
static void printStatusBit(const char *name, uint16_t status, uint16_t mask);

static void printDhtTelemetryStatus(uint16_t status);
static void printLdrTelemetryStatus(uint16_t status);
static void printTelemetrySystemStatus(uint16_t status);
static void printSystemState(uint16_t systemState);

// ---------------------------------
void initMonitor() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
}

void handleMonitor(const uint16_t &systemState, const uint16_t &buttonsState,
                   const uint16_t &ledState) {

#if defined(DEBUG_MODE)

  Serial.println("================================");
  Serial.println("| !!!!!!!! DEBUG MODE !!!!!!!! |");
  // Serial.println("|  Comment out ''DEBUG_MODE''  |");
  // Serial.println("|    in the configuration      |");
  // Serial.println("| to switch to Production mode |");
  Serial.println("================================");

  Serial.println();

#endif
}

#if defined(DEBUG_MODE)
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
}
#endif

static void printJsonPretty(const JsonDocument &doc) {
  serializeJsonPretty(doc, Serial);
  Serial.println();
}

static void printStatusBit(const char *name, uint16_t status, uint16_t mask) {

  uint8_t bit = 0;
  uint16_t temp = mask;

  while (temp > 1) {
    temp >>= 1;
    bit++;
  }

  Serial.print(" (");
  if (bit < 10) {
    Serial.print(" ");
  }
  Serial.print(bit);
  Serial.print(") ");
  Serial.print(name);

  const uint8_t statusColumn = 30;
  uint8_t currentColumn = 6 + strlen(name);

  while (currentColumn < statusColumn) {
    Serial.print(" ");
    currentColumn++;
  }
  Serial.println((status & mask) ? "ON" : "OFF");
}

static void printStatusByte(uint16_t status) {
  Serial.print("  STATUS BYTE: 0b");
  for (int8_t bit = 15; bit >= 0; --bit) {
    if (bit == 7) {
      Serial.println();
      Serial.print("               0b");
    }
    Serial.print((status >> bit) & 1);
  }
  Serial.println();
}

static void printSystemState(uint16_t systemState) {
  Serial.println("[SYSTEM STATE REGISTER]");
  printStatusByte(systemState);

  printStatusBit("LIGHT_COMMAND", systemState, SYSTEM_COMMAND_MASK);
  printStatusBit("SILENT_MODE", systemState, SYSTEM_SILENT_MASK);
  printStatusBit("MQTT_ERR", systemState, SYSTEM_MQTT_ERR_MASK);
  printStatusBit("WIFI_ERR", systemState, SYSTEM_WIFI_ERR_MASK);
}
