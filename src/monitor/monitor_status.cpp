// src/monitor/monitor_status.cpp

#include "monitor_status.h"

#include <Arduino.h>

#include "../buttons/buttons.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"

// ---------------------------------

struct StatusItem {
  const char *name;
  uint8_t mask;
  const char *active;
  const char *inactive;
};

// ---------------------------------

static void printStatusItems(const StatusItem *items, size_t count,
                             const uint8_t &status) {
  for (size_t i = 0; i < count; ++i) {
    Serial.print("  ");
    Serial.print(items[i].name);
    Serial.print(" ");
    Serial.println((status & items[i].mask) ? items[i].active
                                            : items[i].inactive);
  }
}

// ---------------------------------

void printTelemetryStatus(const uint8_t &status) {
  static const StatusItem items[] = {
      {"SILENT_MODE:", STATUS_DEVICE_SILENT_MODE, "SILENT (1)", "NORM (0)"},
      {"LDR_ERR:", STATUS_LDR_ERR, "ERROR (1)", "OK (0)"},
      {"DHT_ERR:", STATUS_DHT_ERR, "ERROR (1)", "OK (0)"},
      {"MQTT_ERR:", STATUS_MQTT_ERR, "ERROR (1)", "OK (0)"},
      {"WIFI_ERR:", STATUS_WIFI_ERR, "ERROR (1)", "OK (0)"},
  };

  Serial.println("[TELEMETRY] Status:");

  printStatusItems(items, sizeof(items) / sizeof(items[0]), status);
}

// ---------------------------------

void printLdrStatus(const uint8_t &status) {
  static const StatusItem items[] = {
      {"DEVICE_ERR:", STATUS_LDR_DEVICE_ERR, "ERROR (1)", "OK (0)"},
      {"DATA_STALE:", STATUS_LDR_DATA_STALE, "STALE (1)", "OK (0)"},
      {"DATA_VALID_ERR:", STATUS_LDR_DATA_VALID_ERR, "ERROR (1)", "OK (0)"},
      {"LUX_ALARM_MIN:", STATUS_LDR_LUX_ALARM_MIN, "ALARM (1)", "OK (0)"},
      {"LUX_ALARM_MAX:", STATUS_LDR_LUX_ALARM_MAX, "ALARM (1)", "OK (0)"},
      {"LIGHT_LOW:", STATUS_LDR_LIGHT_LOW, "LOW (1)", "OK (0)"},
  };

  Serial.println("[LDR] Status:");

  printStatusItems(items, sizeof(items) / sizeof(items[0]), status);
}

// ---------------------------------

void printDhtStatus(const uint8_t &status) {
  static const StatusItem items[] = {
      {"DEVICE_ERR:", STATUS_DHT_DEVICE_ERR, "ERROR (1)", "OK (0)"},
      {"DATA_STALE:", STATUS_DHT_DATA_STALE, "STALE (1)", "OK (0)"},
      {"DATA_VALID_ERR:", STATUS_DHT_DATA_VALID_ERR, "ERROR (1)", "OK (0)"},
      {"TEMPERATURE_ALARM_MIN:", STATUS_DHT_TEMPERATURE_ALARM_MIN, "ALARM (1)",
       "OK (0)"},
      {"TEMPERATURE_ALARM_MAX:", STATUS_DHT_TEMPERATURE_ALARM_MAX, "ALARM (1)",
       "OK (0)"},
      {"HUMIDITY_ALARM_MIN:", STATUS_DHT_HUMIDITY_ALARM_MIN, "ALARM (1)",
       "OK (0)"},
      {"HUMIDITY_ALARM_MAX:", STATUS_DHT_HUMIDITY_ALARM_MAX, "ALARM (1)",
       "OK (0)"},
  };

  Serial.println("[DHT] Status:");

  printStatusItems(items, sizeof(items) / sizeof(items[0]), status);
}

// ---------------------------------

void printButtonsState(const uint8_t &state) {
  static const StatusItem items[] = {
      {"BUTTON_0:", BUTTON_LIGHT_MASK, "ON (1)", "OFF (0)"},
      {"BUTTON_1:", BUTTON_SILENT_MASK, "ON (1)", "OFF (0)"},
      {"BUTTON_2:", BUTTON_WIFI_DISABLE_MASK, "ON (1)", "OFF (0)"},
  };

  Serial.println("[BUTTONS] State:");

  printStatusItems(items, sizeof(items) / sizeof(items[0]), state);
}

// ---------------------------------

void printLedState(const uint8_t &state) {
  static const StatusItem items[] = {
      {"LIGHT_MANUAL:", LED_LIGHT_MANUAL_MASK, "ON (1)", "OFF (0)"},
      {"LIGHT_AUTO:", LED_LIGHT_AUTO_MASK, "ON (1)", "OFF (0)"},
      {"LIGHT_MIN:", LED_LIGHT_MIN_MASK, "ON (1)", "OFF (0)"},
      {"LIGHT_MAX:", LED_LIGHT_MAX_MASK, "ON (1)", "OFF (0)"},
      {"TEMPERATURE_MIN:", LED_TEMPERATURE_MIN, "ON (1)", "OFF (0)"},
      {"TEMPERATURE_MAX:", LED_TEMPERATURE_MAX_MASK, "ON (1)", "OFF (0)"},
      {"HUMIDITY_MIN:", LED_HUMIDITY_MIN_MASK, "ON (1)", "OFF (0)"},
      {"SILENT:", LED_SILENT_MASK, "ON (1)", "OFF (0)"},
  };

  Serial.println("[SYSTEM]    State:");

  printStatusItems(items, sizeof(items) / sizeof(items[0]), state);
}
