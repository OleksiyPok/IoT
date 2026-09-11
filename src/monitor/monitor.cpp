// src/monitor/monitor.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "../time/time.h"
#include "monitor.h"

// ---------------------------------

static void printJsonPretty(const JsonDocument &doc);
static void printStatusBit(const char *name, uint8_t status, uint8_t mask);
static void printDhtStatus(uint8_t status);
static void printLdrStatus(uint8_t status);
static void printSystemStatus(uint8_t status);

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
}

void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &ledState) {

#if defined(DEBUG_MODE)

  Serial.println("================================");
  Serial.println("| !!!!!!!! DEBUG MODE !!!!!!!! |");
  // Serial.println("|  Comment out ''DEBUG_MODE''  |");
  // Serial.println("|    in the configuration      |");
  // Serial.println("| to switch to Production mode |");
  Serial.println("================================");

  printDhtStatus(data.dht.status);
  Serial.println();

  printLdrStatus(data.ldr.status);
  Serial.println();

  printSystemStatus(data.status);

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

  // Serial.println("------------");
}

static void printJsonPretty(const JsonDocument &doc) {
  serializeJsonPretty(doc, Serial);
  Serial.println();
}

static void printStatusBit(const char *name, uint8_t status, uint8_t mask) {
  uint8_t bit = 0;
  uint8_t temp = mask;
  while (temp > 1) {
    temp >>= 1;
    bit++;
  }
  Serial.print(" (");
  Serial.print(bit);
  Serial.print(") ");
  Serial.print(name);
  const uint8_t statusColumn = 30;
  uint8_t currentColumn = 7 + strlen(name);
  while (currentColumn < statusColumn) {
    Serial.print(" ");
    currentColumn++;
  }
  Serial.println((status & mask) ? "ON" : "OFF");
}

static void printStatusByte(uint8_t status) {
  Serial.print("  STATUS BYTE: 0b");
  for (int8_t bit = 7; bit >= 0; --bit) {
    Serial.print((status >> bit) & 1);
  }
  Serial.println();
}

// ---------------------------------

static void printDhtStatus(uint8_t status) {
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

// ---------------------------------

static void printLdrStatus(uint8_t status) {
  Serial.println("[LDR STATUS]");
  printStatusByte(status);

  printStatusBit("DEVICE_ERR", status, STATUS_LDR_DEVICE_ERR);
  printStatusBit("DATA_STALE", status, STATUS_LDR_DATA_STALE);
  printStatusBit("DATA_VALID_ERR", status, STATUS_LDR_DATA_VALID_ERR);

  printStatusBit("LUX_ALARM_MIN", status, STATUS_LDR_LUX_ALARM_MIN);
  printStatusBit("LUX_ALARM_MAX", status, STATUS_LDR_LUX_ALARM_MAX);
  printStatusBit("LIGHT_LOW", status, STATUS_LDR_LIGHT_LOW);
}

// ---------------------------------

static void printSystemStatus(uint8_t status) {
  Serial.println("[SYSTEM STATUS]");
  printStatusByte(status);

  printStatusBit("DEVICE_SILENT_MODE", status, STATUS_DEVICE_SILENT_MODE);
  printStatusBit("LDR_ERR", status, STATUS_LDR_ERR);
  printStatusBit("DHT_ERR", status, STATUS_DHT_ERR);
  printStatusBit("MQTT_ERR", status, STATUS_MQTT_ERR);
  printStatusBit("WIFI_ERR", status, STATUS_WIFI_ERR);
}