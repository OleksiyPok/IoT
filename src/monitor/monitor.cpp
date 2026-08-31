// src/monitor/monitor.cpp

#include <Arduino.h>

#include "monitor.h"
#include "../buttons/buttons.h"
#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "esp_mac.h"

// ---------------------------------

static void printDeviceId(const Telemetry &data);
static void printTelemetryTimestamp(const Telemetry &data);
static void printTelemetryUptime(const Telemetry &data);
static void printDhdData(const Telemetry &data);
static void printLdrData(const Telemetry &data);
static void printDhtStatus(const uint8_t &status);
static void printLdrStatus(const uint8_t &status);
static void printTelemetryStatus(const uint8_t &status);
static void printButtonsState(const uint8_t &state);
static void printLedState(const uint8_t &state);

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println("=== Device ESP32 started ===");
  Serial.println();
};

void handleMonitor(const Telemetry &data) {
  printDeviceId(data);
  // printTelemetryTimestamp(data);
  printTelemetryUptime(data);
  printTelemetryStatus(data.status);
  Serial.println();
  printLdrData(data);
  printLdrStatus(data.ldr.status);
  Serial.println();
  printDhdData(data);
  printDhtStatus(data.dht.status);
  // printButtonsState(buttonsState);
  // printLedState(ledState);
  Serial.println("------------");
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

static void printDhdData(const Telemetry &data) {
  Serial.print("[DHT] Temperature: ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(data.dht.humidity, 0);
  Serial.println("%");
};

static void printLdrData(const Telemetry &data) {
  Serial.print("[LDR] ADC: ");
  Serial.print(data.ldr.raw);
  Serial.print(", Lux: ");
  Serial.println(data.ldr.lux, 1);
};

static void printTelemetryStatus(const uint8_t &status) {
  Serial.println("[TELEMETRY] Status:");

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

  Serial.print("  DATA_VALID_ERR:         ");
  Serial.println((status & STATUS_LDR_DATA_VALID_ERR) ? "ERROR" : "OK");

  Serial.print("  LUX_ALARM_MIN:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MIN) ? "ALARM" : "OK");

  Serial.print("  LUX_ALARM_MAX:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MAX) ? "ALARM" : "OK");
}

static void printDhtStatus(const uint8_t &status) {
  Serial.println("[DHT] Status:");

  Serial.print("  DEVICE_ERR:             ");
  Serial.println((status & STATUS_DHT_DEVICE_ERR) ? "ERROR" : "OK");

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

  Serial.print("  BUTTON_0:               ");
  Serial.println((state & BUTTON_LIGHT_MASK) ? "ON" : "OFF");

  Serial.print("  BUTTON_1:               ");
  Serial.println((state & BUTTON_SERIAL_MONITOR_MASK) ? "ON" : "OFF");

  Serial.print("  BUTTON_2:               ");
  Serial.println((state & BUTTON_WIFI_DISABLE) ? "ON" : "OFF");
}

static void printLedState(const uint8_t &state) {
  Serial.println("[LED] State:");

  Serial.print("  LIGHT_MANUAL:    ");
  Serial.println((state & LED_LIGHT_MANUAL) ? "ON" : "OFF");

  Serial.print("  LIGHT_AUTO:      ");
  Serial.println((state & LED_LIGHT_AUTO) ? "ON" : "OFF");

  Serial.print("  LIGHT_MIN:       ");
  Serial.println((state & LED_LIGHT_MIN) ? "ON" : "OFF");

  Serial.print("  LIGHT_MAX:       ");
  Serial.println((state & LED_LIGHT_MAX) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MIN: ");
  Serial.println((state & LED_TEMPERATURE_MIN) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MAX: ");
  Serial.println((state & LED_TEMPERATURE_MAX) ? "ON" : "OFF");

  Serial.print("  HUMIDITY_MIN:    ");
  Serial.println((state & LED_HUMIDITY_MIN) ? "ON" : "OFF");

  Serial.print("  SERIAL_MONITOR:  ");
  Serial.println((state & LED_SERIAL_MONITOR) ? "ON" : "OFF");
}