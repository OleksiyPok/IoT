// src/monitor/monitor.cpp

#include "monitor.h"
#include "../config.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../main.h"
#include <Arduino.h>

// ---------------------------------

static void printDhtStatus(const uint8_t status);
static void printLdrStatus(const uint8_t status);
static void printTelemetryStatus(const uint8_t &status);
static void printLedState(const uint8_t &state);

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Device started ===");
  Serial.println();
};

void handleMonitor(const Telemetry &data) {
  Serial.print("[DHT] Temperature: ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(data.dht.humidity, 0);
  Serial.print("%");
  Serial.println();

  Serial.print("[LDR] ADC: ");
  Serial.print(data.ldr.raw);
  Serial.print(", Lux: ");
  Serial.print(data.ldr.lux, 1);
  Serial.println();

  printTelemetryStatus(data.status);
  printLdrStatus(data.ldr.status);
  printDhtStatus(data.dht.status);
  printLedState(ledState);
  Serial.println("------------");
}

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

static void printLdrStatus(uint8_t status) {
  Serial.println("[LDR] Status:");

  Serial.print("  DEVICE_ERROR:           ");
  Serial.println((status & STATUS_LDR_DEVICE_ERROR) ? "ERROR" : "OK");

  Serial.print("  DATA_VALID_ERROR:       ");
  Serial.println((status & STATUS_LDR_DATA_VALID_ERROR) ? "ERROR" : "OK");

  Serial.print("  LUX_ALARM_MIN:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MIN) ? "ALARM" : "OK");

  Serial.print("  LUX_ALARM_MAX:          ");
  Serial.println((status & STATUS_LDR_LUX_ALARM_MAX) ? "ALARM" : "OK");
}

static void printDhtStatus(uint8_t status) {
  Serial.println("[DHT] Status:");

  Serial.print("  DEVICE_ERROR:           ");
  Serial.println((status & STATUS_DHT_DEVICE_ERROR) ? "ERROR" : "OK");

  Serial.print("  DATA_VALID_ERROR:       ");
  Serial.println((status & STATUS_DHT_DATA_VALID_ERROR) ? "ERROR" : "OK");

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