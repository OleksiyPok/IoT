// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"

// ---------------------------------
static uint8_t previousButtonsState = 0;

static void updateLedState(const uint8_t &buttonsState, uint8_t &systemState);
static void updateDhtStatus(const DHTData &data, uint8_t &systemState);
static void updateLdrStatus(const LDRData &data, uint8_t &systemState);

// ---------------------------------

void handleActions(const Telemetry &telemetryData, uint8_t &buttonsState,
                   uint8_t &systemState) {

  updateLedState(buttonsState, systemState);
  updateLdrStatus(telemetryData.ldr, systemState);
  updateDhtStatus(telemetryData.dht, systemState);
}

static void updateLedState(const uint8_t &buttonsState, uint8_t &systemState) {

  if (buttonsState & BUTTON_LIGHT_MASK) {
    systemState |= LED_LIGHT_MANUAL;
  } else {
    systemState &= ~LED_LIGHT_MANUAL;
  }

  if (buttonsState & BUTTON_SILENT_MASK) {
    systemState |= LED_SILENT;
  } else {
    systemState &= ~LED_SILENT;
  }

  if ((buttonsState & BUTTON_WIFI_DISABLE) &&
      !(previousButtonsState & BUTTON_WIFI_DISABLE)) {
    Serial.println("[TEST] WiFi disconnect");
    disconnectWiFi();
  }

  previousButtonsState = buttonsState;
};

static void updateDhtStatus(const DHTData &data, uint8_t &systemState) {
  systemState &= ~(LED_TEMPERATURE_MIN | LED_TEMPERATURE_MAX | LED_HUMIDITY_MIN);

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MIN) {
    systemState |= LED_TEMPERATURE_MIN;
  }

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MAX) {
    systemState |= LED_TEMPERATURE_MAX;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MIN) {
    systemState |= LED_HUMIDITY_MIN;
  }

  // if (data.status & STATUS_DHT_HUMIDITY_ALARM_MAX) {
  //   systemState |= LED_HUMIDITY_MAX;
  // }
}

static void updateLdrStatus(const LDRData &data, uint8_t &systemState) {
  systemState &= ~(LED_LIGHT_MIN | LED_LIGHT_MAX | LED_LIGHT_AUTO);

  if (data.status & STATUS_LDR_LUX_ALARM_MIN) {
    systemState |= LED_LIGHT_MIN;
  }

  if (data.status & STATUS_LDR_LUX_ALARM_MAX) {
    systemState |= LED_LIGHT_MAX;
  }

  if (data.status & STATUS_LDR_LIGHT_LOW) {
    systemState |= LED_LIGHT_AUTO;
  }
}
