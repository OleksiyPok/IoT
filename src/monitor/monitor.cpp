// src/monitor/monitor.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "../time/time.h"
#include "monitor.h"

// ---------------------------------
static void printJsonPretty(const JsonDocument &doc);
static void printStatusBit(const char *name, uint16_t status, uint16_t mask);

static void printDhtTelemetryStatus(uint16_t status);
static void printLdrTelemetryStatus(uint16_t status);
static void printTelemetrySystemStatus(uint16_t status);
static void printSystemState(const Telemetry &telemetry);

// ---------------------------------
void initMonitor() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
}

void handleMonitor(const Telemetry &telemetry) {

#if defined(DEBUG_MODE)

  Serial.println("================================");
  Serial.println("| !!!!!!!! DEBUG MODE !!!!!!!! |");
  // Serial.println("|  Comment out ''DEBUG_MODE''  |");
  // Serial.println("|    in the configuration      |");
  // Serial.println("| to switch to Production mode |");
  Serial.println("================================");

  printDhtTelemetryStatus(telemetry.dht.status);
  Serial.println();

  printLdrTelemetryStatus(telemetry.ldr.status);
  Serial.println();

  printTelemetrySystemStatus(telemetry.status);
  Serial.println();

  printSystemState(telemetry);

  Serial.println("================================");
  Serial.println();

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
}

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

static void printSystemState(const Telemetry &telemetry) {
  Serial.println("[SYSTEM STATE REGISTER]");
  printStatusByte(telemetry.systemState);

  printStatusBit("LIGHT_COMMAND", telemetry.systemState, SYSTEM_COMMAND_MASK);
  printStatusBit("SILENT_MODE", telemetry.systemState, SYSTEM_SILENT_MASK);
  printStatusBit("LDR_ERR", telemetry.systemState, SYSTEM_LDR_ERR_MASK);
  printStatusBit("DHT_ERR", telemetry.systemState, SYSTEM_DHT_ERR_MASK);
  printStatusBit("MQTT_ERR", telemetry.systemState, SYSTEM_MQTT_ERR_MASK);
  printStatusBit("WIFI_ERR", telemetry.systemState, SYSTEM_WIFI_ERR_MASK);
}

static void printDhtTelemetryStatus(uint16_t status) {
  Serial.println("[DHT STATUS]");
  printStatusByte(status);

  printStatusBit("DEVICE_ERR", status, STATUS_DHT_DEVICE_ERR);
  printStatusBit("DATA_STALE", status, STATUS_DHT_DATA_STALE);
  printStatusBit("DATA_VALID_ERR", status, STATUS_DHT_DATA_VALID_ERR);

  printStatusBit("TEMPERATURE_ALARM_MIN", status,
                 STATUS_DHT_TEMPERATURE_ALARM_MIN);
  printStatusBit("TEMPERATURE_ALARM_MAX", status,
                 STATUS_DHT_TEMPERATURE_ALARM_MAX);

  printStatusBit("HUMIDITY_ALARM_MIN", status, STATUS_DHT_HUMIDITY_ALARM_MIN);
  printStatusBit("HUMIDITY_ALARM_MAX", status, STATUS_DHT_HUMIDITY_ALARM_MAX);
}

static void printLdrTelemetryStatus(uint16_t status) {
  Serial.println("[LDR STATUS]");
  printStatusByte(status);

  printStatusBit("DEVICE_ERR", status, STATUS_LDR_DEVICE_ERR);
  printStatusBit("DATA_STALE", status, STATUS_LDR_DATA_STALE);
  printStatusBit("DATA_VALID_ERR", status, STATUS_LDR_DATA_VALID_ERR);

  printStatusBit("LUX_ALARM_MIN", status, STATUS_LDR_LUX_ALARM_MIN);
  printStatusBit("LUX_ALARM_MAX", status, STATUS_LDR_LUX_ALARM_MAX);
  printStatusBit("LIGHT_LOW", status, STATUS_LDR_LIGHT_LOW);
}

static void printTelemetrySystemStatus(uint16_t status) {
  Serial.println("[SYSTEM STATUS]");
  printStatusByte(status);

  printStatusBit("DEVICE_SILENT_MODE", status, STATUS_DEVICE_SILENT_MODE);
  printStatusBit("LDR_ERR", status, STATUS_LDR_ERR);
  printStatusBit("DHT_ERR", status, STATUS_DHT_ERR);
  printStatusBit("MQTT_ERR", status, STATUS_MQTT_ERR);
  printStatusBit("WIFI_ERR", status, STATUS_WIFI_ERR);
}
