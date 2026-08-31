// src/actions/actions.cpp

#include <WiFi.h>

#include "../buttons/buttons.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "actions.h"

// ---------------------------------

static void updateDhtStatus(const DHTData &data, uint8_t &ledState);
static void updateLdrStatus(const LDRData &data, uint8_t &ledState);

// ---------------------------------

void handleActions(const Telemetry &telemetryData, uint8_t &buttonsState,
                   uint8_t &ledState) {

  if (buttonsState & BUTTON_LIGHT_MASK) {
    ledState |= LED_LIGHT_MANUAL;
  } else {
    ledState &= ~LED_LIGHT_MANUAL;
  }

  if (buttonsState & BUTTON_SERIAL_MONITOR_MASK) {
    ledState |= LED_SERIAL_MONITOR;
  } else {
    ledState &= ~LED_SERIAL_MONITOR;
  }

  if (buttonsState & BUTTON_WIFI_DISABLE) {
    // Serial.println("~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("[TEST] WiFi disconnect");
    // Serial.println("~~~~~~~~~~~~~~~~~~~~~~");
    WiFi.disconnect();
  }

  updateLdrStatus(telemetryData.ldr, ledState);
  updateDhtStatus(telemetryData.dht, ledState);
}

static void updateDhtStatus(const DHTData &data, uint8_t &ledState) {
  ledState &= ~(LED_TEMPERATURE_MIN | LED_TEMPERATURE_MAX | LED_HUMIDITY_MIN);

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MIN) {
    ledState |= LED_TEMPERATURE_MIN;
  }

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MAX) {
    ledState |= LED_TEMPERATURE_MAX;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MIN) {
    ledState |= LED_HUMIDITY_MIN;
  }

  // if (data.status & STATUS_DHT_HUMIDITY_ALARM_MAX) {
  //   ledState |= LED_HUMIDITY_MAX;
  // }
}

static void updateLdrStatus(const LDRData &data, uint8_t &ledState) {
  ledState &= ~(LED_LIGHT_MIN | LED_LIGHT_MAX | LED_LIGHT_AUTO);

  if (data.status & STATUS_LDR_LUX_ALARM_MIN) {
    ledState |= LED_LIGHT_MIN;
  }

  if (data.status & STATUS_LDR_LUX_ALARM_MAX) {
    ledState |= LED_LIGHT_MAX;
  }

  if (data.status & STATUS_LDR_LIGHT_LOW) {
    ledState |= LED_LIGHT_AUTO;
  }
}
