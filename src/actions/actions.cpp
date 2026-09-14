// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../commands/commands.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"

// ---------------------------------
static uint16_t previousButtonsState = 0x0000;

static void updateLedState(const uint16_t &systemState, uint16_t &ledState);
static void updateSystemState(const uint16_t &buttonsState,
                              uint16_t &systemState);
static void updateDhtStatus(const DHTData &data, uint16_t &ledState);
static void updateLdrStatus(const LDRData &data, uint16_t &ledState);
static void handleWifiTest(const uint16_t &buttonsState);
static void handleCommand(const uint16_t &buttonsState,
                          CommandQueue &commandQueue);

// ---------------------------------

void handleActions(const Telemetry &telemetryData, const uint16_t &buttonsState,
                   uint16_t &systemState, uint16_t &ledState,
                   CommandQueue &commandQueue) {

  updateSystemState(buttonsState, systemState);
  updateLedState(systemState, ledState);
  handleCommand(buttonsState, commandQueue);
  handleWifiTest(buttonsState);
  updateLdrStatus(telemetryData.ldr, ledState);
  updateDhtStatus(telemetryData.dht, ledState);

  previousButtonsState = buttonsState;
}

void handleActions(const Telemetry &telemetryData, const uint16_t &buttonsState,
                   uint16_t &systemState, uint16_t &ledState) {

  updateSystemState(buttonsState, systemState);
  updateLedState(systemState, ledState);
  updateLdrStatus(telemetryData.ldr, ledState);
  updateDhtStatus(telemetryData.dht, ledState);
}

static void updateSystemState(const uint16_t &buttonsState,
                              uint16_t &systemState) {
  if (buttonsState & BUTTON_COMMAND_MASK)
    systemState |= SYSTEM_COMMAND_MASK;
  else
    systemState &= ~SYSTEM_COMMAND_MASK;

  if (buttonsState & BUTTON_SILENT_MASK)
    systemState |= SYSTEM_SILENT_MASK;
  else
    systemState &= ~SYSTEM_SILENT_MASK;
}

static void handleWifiTest(const uint16_t &buttonsState) {

  if ((buttonsState & BUTTON_WIFI_DISABLE_MASK) &&
      !(previousButtonsState & BUTTON_WIFI_DISABLE_MASK)) {
    Serial.println("[TEST] WiFi disconnect");
    disconnectWiFi();
  }

  previousButtonsState = buttonsState;
}

static void updateLedState(const uint16_t &systemState, uint16_t &ledState) {

  if (systemState & SYSTEM_COMMAND_MASK) {
    ledState |= LED_COMMAND_MASK;
  } else {
    ledState &= ~LED_COMMAND_MASK;
  }

  if (systemState & SYSTEM_SILENT_MASK)
    ledState |= LED_SILENT_MASK;
  else
    ledState &= ~LED_SILENT_MASK;
}

static void updateDhtStatus(const DHTData &data, uint16_t &ledState) {
  ledState &= ~(LED_TEMPERATURE_MIN_MASK | LED_TEMPERATURE_MAX_MASK |
                LED_HUMIDITY_MIN_MASK);

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MIN) {
    ledState |= LED_TEMPERATURE_MIN_MASK;
  }

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MAX) {
    ledState |= LED_TEMPERATURE_MAX_MASK;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MIN) {
    ledState |= LED_HUMIDITY_MIN_MASK;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MAX) {
    ledState |= LED_HUMIDITY_MAX_MASK;
  }
}

static void updateLdrStatus(const LDRData &data, uint16_t &ledState) {
  ledState &= ~(LED_LIGHT_MIN_MASK | LED_LIGHT_MAX_MASK | LED_LIGHT_AUTO_MASK);

  if (data.status & STATUS_LDR_LUX_ALARM_MIN) {
    ledState |= LED_LIGHT_MIN_MASK;
  }

  if (data.status & STATUS_LDR_LUX_ALARM_MAX) {
    ledState |= LED_LIGHT_MAX_MASK;
  }

  if (data.status & STATUS_LDR_LIGHT_LOW) {
    ledState |= LED_LIGHT_AUTO_MASK;
  }
}

static void handleCommand(const uint16_t &buttonsState,
                          CommandQueue &commandQueue) {

  if ((buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    enqueueCommand(commandQueue, MANUAL_READ_COMMAND);
  }
}
