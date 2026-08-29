// src/monitor/monitor.cpp

#include "monitor.h"
#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "esp_mac.h"
#include <Arduino.h>

// ---------------------------------

static void printDeviceId(const Telemetry &data);
static void printTelemetryWorktime(const Telemetry &data);
static void printDhdData(const Telemetry &data);
static void printLdrData(const Telemetry &data);
static void printDhtStatus(const uint8_t &status);
static void printLdrStatus(const uint8_t &status);
static void printTelemetryStatus(const uint8_t &status);
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
  printTelemetryWorktime(data);
  printTelemetryStatus(data.status);
  // printLdrData(data);
  // printLdrStatus(data.ldr.status);
  // printDhdData(data);
  // printDhtStatus(data.dht.status);
  // printLedState(ledState);
  Serial.println("------------");
}

static void printDeviceId(const Telemetry &data) {
  Serial.print("[Device] MAC: ");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                (uint8_t)(data.deviceId >> 40), (uint8_t)(data.deviceId >> 32),
                (uint8_t)(data.deviceId >> 24), (uint8_t)(data.deviceId >> 16),
                (uint8_t)(data.deviceId >> 8), (uint8_t)data.deviceId);
  // Serial.println(data.deviceId);
}

static void printTelemetryWorktime(const Telemetry &data) {
  Serial.print("[TELEMETRY] Worktime: ");
  Serial.print(data.worktime);
  Serial.println(" sek");
};

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

static void printLedState(const uint8_t &state) {
  Serial.println("[LED] State:");

  Serial.print("  LIGHT_MANUAL:           ");
  Serial.println((state & LED_LIGHT_MANUAL) ? "ON" : "OFF");

  Serial.print("  LIGHT_AUTO:             ");
  Serial.println((state & LED_LIGHT_AUTO) ? "ON" : "OFF");

  Serial.print("  LIGHT_MIN:              ");
  Serial.println((state & LED_LIGHT_MIN) ? "ON" : "OFF");

  Serial.print("  LIGHT_MAX:              ");
  Serial.println((state & LED_LIGHT_MAX) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MIN:        ");
  Serial.println((state & LED_TEMPERATURE_MIN) ? "ON" : "OFF");

  Serial.print("  TEMPERATURE_MAX:        ");
  Serial.println((state & LED_TEMPERATURE_MAX) ? "ON" : "OFF");

  Serial.print("  HUMIDITY_MIN:           ");
  Serial.println((state & LED_HUMIDITY_MAX) ? "ON" : "OFF");

  Serial.print("  HUMIDITY_MAX:           ");
  Serial.println((state & LED_HUMIDITY_MAX) ? "ON" : "OFF");
}