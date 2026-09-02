// src/monitor/monitor.cpp

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../buttons/buttons.h"
#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "esp_mac.h"
#include "monitor.h"

// ---------------------------------

static void printDeviceId(const Telemetry &data);
static void printTelemetryTimestamp(const Telemetry &data);
static void printTelemetryUptime(const Telemetry &data);
static void printTelemetrySequence(const Telemetry &data);
static void printTelemetryData(const Telemetry &data);
static void printDhdData(const Telemetry &data);
static void printLdrData(const Telemetry &data);
static void printDhtStatus(const uint8_t &status);
static void printLdrStatus(const uint8_t &status);
static void printTelemetryStatus(const uint8_t &status);
static void printButtonsState(const uint8_t &state);
static void printLedState(const uint8_t &state);

static void printTelemetryPayload(const JsonDocument &doc);
static void printPayloadStatus(const JsonDocument &doc);

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
};

void handleMonitor(const Telemetry &data, const uint8_t &buttonsState,
                   const uint8_t &systemState) {
#if PRINT_MODE == PRINT_MODE_DEVICEID
  printTelemetryUptime(data);
  printDeviceId(data);
#elif PRINT_MODE == PRINT_MODE_TIMESTAMP
  printTelemetryTimestamp(data);
#elif PRINT_MODE == PRINT_MODE_UPTIME
  printTelemetryUptime(data);
#elif PRINT_MODE == PRINT_MODE_TELEMETRY_DATA
  printDeviceId(data);
  printTelemetryTimestamp(data);
  printTelemetryUptime(data);
  printLdrData(data);
  printDhdData(data);
#elif PRINT_MODE == PRINT_MODE_TELEMETRY_STATUS
  printTelemetryUptime(data);
  printTelemetryStatus(data.status);
#elif PRINT_MODE == PRINT_MODE_TELEMETRY_DATA_STATUS
  printTelemetryData(data);
  printTelemetryStatus(data.status);
#elif PRINT_MODE == PRINT_MODE_LDR_DATA_STATUS
  printTelemetryUptime(data);
  printLdrData(data);
  printLdrStatus(data.ldr.status);
#elif PRINT_MODE == PRINT_MODE_DHT_DATA_STATUS
  printTelemetryUptime(data);
  printDhdData(data);
  printDhtStatus(data.dht.status);
#elif PRINT_MODE == PRINT_MODE_LDR_DATA_STATUS_DHD_DATA_STATUS
  printTelemetryUptime(data);
  printDhdData(data);
  printDhtStatus(data.dht.status);
  printLdrData(data);
  printLdrStatus(data.ldr.status);
#elif PRINT_MODE == PRINT_MODE_ALL_STATUS
  printTelemetryUptime(data);
  printTelemetryStatus(data.status);
  printLdrStatus(data.ldr.status);
  printDhtStatus(data.dht.status);
#elif PRINT_MODE == PRINT_MODE_BUTTON_STATE_LED_STATE
  printTelemetryUptime(data);
  printButtonsState(buttonsState);
  printLedState(systemState);
#else
  printDeviceId(data);
  printTelemetryTimestamp(data);
  printTelemetryUptime(data);
  printTelemetryStatus(data.status);
  Serial.println();
  printLdrData(data);
  printLdrStatus(data.ldr.status);
  Serial.println();
  printDhdData(data);
  printDhtStatus(data.dht.status);
  printButtonsState(buttonsState);
  printLedState(systemState);
#endif
  Serial.println("------------");
}

void handleMonitorPayload(const char *payload) {
  if (payload == nullptr) {
    Serial.println("[MONITOR] Payload is null");
    return;
  }

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("[MONITOR] JSON parse error: ");
    Serial.println(error.c_str());
    return;
  }

  Serial.println("[MONITOR] HTTP payload:");
  Serial.println("↓↓↓↓↓↓↓↓↓↓↓");

  printTelemetryPayload(doc);
  printPayloadStatus(doc);

  Serial.println("------------");
}

static void printTelemetryPayload(const JsonDocument &doc) {
  Serial.print("[TELEMETRY] DeviceId (MAC): ");

  uint64_t deviceId = doc["deviceId"].as<uint64_t>();

  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", (uint8_t)(deviceId >> 40),
                (uint8_t)(deviceId >> 32), (uint8_t)(deviceId >> 24),
                (uint8_t)(deviceId >> 16), (uint8_t)(deviceId >> 8),
                (uint8_t)deviceId);

  uint32_t timestamp = doc["timestamp"].as<uint32_t>();

  time_t timeValue = timestamp;
  struct tm timeInfo;

  gmtime_r(&timeValue, &timeInfo);

  Serial.printf(
      "[TELEMETRY] Timestamp: %04d-%02d-%02d %02d:%02d:%02d UTC (%lu)\r\n",
      timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
      timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
      static_cast<unsigned long>(timestamp));

  uint32_t uptime = doc["uptime"].as<uint32_t>();

  uint32_t hours = uptime / 3600;
  uint32_t minutes = (uptime % 3600) / 60;
  uint32_t seconds = uptime % 60;

  Serial.printf(
      "[TELEMETRY] Uptime: %02lu:%02lu:%02lu (%lu sec)\r\n",
      static_cast<unsigned long>(hours), static_cast<unsigned long>(minutes),
      static_cast<unsigned long>(seconds), static_cast<unsigned long>(uptime));

  Serial.print("[TELEMETRY] Sequence: ");
  Serial.println(doc["sequence"].as<uint8_t>());

  Serial.print("[LDR] ADC: ");
  Serial.print(doc["ldr"]["raw"].as<uint16_t>());
  Serial.print(", Lux: ");
  Serial.println(doc["ldr"]["lux"].as<float>(), 1);

  Serial.print("[LDR] Updated: ");
  Serial.println(doc["ldr"]["updated"].as<uint32_t>());

  Serial.print("[DHT] Temperature: ");
  Serial.print(doc["dht"]["temperature"].as<float>(), 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(doc["dht"]["humidity"].as<float>(), 0);
  Serial.println("%");

  Serial.print("[DHT] Updated: ");
  Serial.println(doc["dht"]["updated"].as<uint32_t>());
}

static void printPayloadStatus(const JsonDocument &doc) {
  uint8_t status = doc["status"].as<uint8_t>();
  uint8_t ldrStatus = doc["ldr"]["status"].as<uint8_t>();
  uint8_t dhtStatus = doc["dht"]["status"].as<uint8_t>();

  Serial.println();
  printTelemetryStatus(status);
  Serial.println();
  printLdrStatus(ldrStatus);
  Serial.println();
  printDhtStatus(dhtStatus);
}

static void printDeviceId(const Telemetry &data) {
  Serial.print("[TELEMETRY] DeviceId (MAC): ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                (uint8_t)(data.deviceId >> 40), (uint8_t)(data.deviceId >> 32),
                (uint8_t)(data.deviceId >> 24), (uint8_t)(data.deviceId >> 16),
                (uint8_t)(data.deviceId >> 8), (uint8_t)data.deviceId);
  // Serial.println(data.deviceId);
}

static void printTelemetryTimestamp(const Telemetry &data) {
  time_t timestamp = data.timestamp;

  struct tm timeInfo;
  gmtime_r(&timestamp, &timeInfo);

  Serial.printf(
      "[TELEMETRY] Timestamp: %04d-%02d-%02d %02d:%02d:%02d UTC (%lu)\r\n",
      timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
      timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
      static_cast<unsigned long>(data.timestamp));
}

static void printTelemetryUptime(const Telemetry &data) {
  uint32_t hours = data.uptime / 3600;
  uint32_t minutes = (data.uptime % 3600) / 60;
  uint32_t seconds = data.uptime % 60;

  Serial.printf("[TELEMETRY] Uptime: %02lu:%02lu:%02lu (%lu sec)\r\n",
                static_cast<unsigned long>(hours),
                static_cast<unsigned long>(minutes),
                static_cast<unsigned long>(seconds),
                static_cast<unsigned long>(data.uptime));
}

static void printTelemetrySequence(const Telemetry &data) {
  Serial.print("[TELEMETRY] Sequence: ");
  Serial.println(data.sequence);
}

static void printTelemetryData(const Telemetry &data) {
  printDeviceId(data);
  printTelemetryTimestamp(data);
  printTelemetryUptime(data);
  printTelemetrySequence(data);
  printLdrData(data);
  printDhdData(data);
}

static void printDhdData(const Telemetry &data) {
  Serial.print("[DHT] Temperature: ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(data.dht.humidity, 0);
  Serial.println("%");

  Serial.print("[DHT] Updated: ");
  Serial.println(data.dht.updated);
};

static void printLdrData(const Telemetry &data) {
  Serial.print("[LDR] ADC: ");
  Serial.print(data.ldr.raw);
  Serial.print(", Lux: ");
  Serial.println(data.ldr.lux, 1);

  Serial.print("[LDR] Updated: ");
  Serial.println(data.ldr.updated);
};

static void printTelemetryStatus(const uint8_t &status) {
  Serial.println("[TELEMETRY] Status:");

  Serial.print("  SILENT_MODE:            ");
  Serial.println((status & STATUS_DEVICE_SILENT_MODE) ? "SILENT" : "NORMAL");

  Serial.print("  LDR_ERR:                ");
  Serial.println((status & STATUS_LDR_ERR) ? "ERROR" : "OK");

  Serial.print("  DHT_ERR:                ");
  Serial.println((status & STATUS_DHT_ERR) ? "ERROR" : "OK");

  Serial.print("  MQTT_ERR:               ");
  Serial.println((status & STATUS_MQTT_ERR) ? "ERROR" : "OK");

  Serial.print("  WIFI_ERR:               ");
  Serial.println((status & STATUS_WIFI_ERR) ? "ERROR" : "OK");
};

static void printLdrStatus(const uint8_t &status) {
  Serial.println("[LDR] Status:");

  Serial.print("  DEVICE_ERR:             ");
  Serial.println((status & STATUS_LDR_DEVICE_ERR) ? "ERROR" : "OK");

  Serial.print("  DATA_STALE:             ");
  Serial.println((status & STATUS_LDR_DATA_STALE) ? "STALE" : "OK");

  Serial.print("  DATA_VALID_ERR:         ");
  Serial.println((status & STATUS_LDR_DATA_VALID_ERR) ? "ERROR" : "OK");

  Serial.print("  LUX_ALARM_MIN:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MIN) ? "ALARM" : "OK");

  Serial.print("  LUX_ALARM_MAX:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MAX) ? "ALARM" : "OK");

  Serial.print("  LIGHT_LOW:              ");
  Serial.println((status & STATUS_LDR_LIGHT_LOW) ? "LOW" : "OK");
};

static void printDhtStatus(const uint8_t &status) {
  Serial.println("[DHT] Status:");

  Serial.print("  DEVICE_ERR:             ");
  Serial.println((status & STATUS_DHT_DEVICE_ERR) ? "ERROR" : "OK");

  Serial.print("  DATA_STALE:             ");
  Serial.println((status & STATUS_DHT_DATA_STALE) ? "STALE" : "OK");

  Serial.print("  DATA_VALID_ERR:         ");
  Serial.println((status & STATUS_DHT_DATA_VALID_ERR) ? "ERROR" : "OK");

  Serial.print("  TEMPERATURE_ALARM_MIN:  ");
  Serial.println((status & STATUS_DHT_TEMPERATURE_ALARM_MIN) ? "ALARM" : "OK");

  Serial.print("  TEMPERATURE_ALARM_MAX:  ");
  Serial.println((status & STATUS_DHT_TEMPERATURE_ALARM_MAX) ? "ALARM" : "OK");

  Serial.print("  HUMIDITY_ALARM_MIN:     ");
  Serial.println((status & STATUS_DHT_HUMIDITY_ALARM_MIN) ? "ALARM" : "OK");

  Serial.print("  HUMIDITY_ALARM_MAX:     ");
  Serial.println((status & STATUS_DHT_HUMIDITY_ALARM_MAX) ? "ALARM" : "OK");
}

static void printButtonsState(const uint8_t &state) {
  Serial.println("[BUTTONS] State:");

  Serial.print("  BUTTON_0:         ");
  Serial.println((state & BUTTON_LIGHT_MASK) ? "ON" : "OFF");

  Serial.print("  BUTTON_1:         ");
  Serial.println((state & BUTTON_SILENT_MASK) ? "ON" : "OFF");

  Serial.print("  BUTTON_2:         ");
  Serial.println((state & BUTTON_WIFI_DISABLE) ? "ON" : "OFF");
}

static void printLedState(const uint8_t &state) {
  Serial.println("[SYSTEM]    State:");

  Serial.print("  LIGHT_MANUAL:     ");
  Serial.println((state & LED_LIGHT_MANUAL) ? "ON" : "OFF");

  Serial.print("  LIGHT_AUTO:       ");
  Serial.println((state & LED_LIGHT_AUTO) ? "ON" : "OFF");

  Serial.print("  LIGHT_MIN:        ");
  Serial.println((state & LED_LIGHT_MIN) ? "ON" : "OFF");

  Serial.print("  LIGHT_MAX:        ");
  Serial.println((state & LED_LIGHT_MAX) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MIN:  ");
  Serial.println((state & LED_TEMPERATURE_MIN) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MAX:  ");
  Serial.println((state & LED_TEMPERATURE_MAX) ? "ON" : "OFF");

  Serial.print("  HUMIDITY_MIN:     ");
  Serial.println((state & LED_HUMIDITY_MIN) ? "ON" : "OFF");

  Serial.print("  SILENT:           ");
  Serial.println((state & LED_SILENT) ? "ON" : "OFF");
}
