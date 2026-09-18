// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../commands/commands.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../mqtt/mqtt_connection.h"
#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"

// ---------------------------------
static uint16_t previousButtonsState = 0x0000;

static void updateSystemState(const Telemetry &telemetry,
                              const uint16_t &buttonsState,
                              uint16_t &systemState);
static void updateDhtStatus(const DHTData &data, uint16_t &ledState);
static void updateLdrStatus(const LDRData &data, uint16_t &ledState);
static void updateLedState(const Telemetry &telemetry,
                           const uint16_t &systemState, uint16_t &ledState);

static void handleWifiTest(const uint16_t &buttonsState);
static void handleCommand(const uint16_t &buttonsState);

// ---------------------------------
void handleActions(const Telemetry &telemetry, const uint16_t &buttonsState,
                   uint16_t &systemState, uint16_t &ledState) {

  updateSystemState(telemetry, buttonsState, systemState);
  updateLedState(telemetry, systemState, ledState);
  handleCommand(buttonsState);
  handleWifiTest(buttonsState);

  previousButtonsState = buttonsState;
}

static void updateSystemState(const Telemetry &telemetry,
                              const uint16_t &buttonsState,
                              uint16_t &systemState) {

  systemState &= ~SYSTEM_STATE_MANAGED_MASK;

  if ((buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    systemState |= SYSTEM_COMMAND_MASK;
  }

  if (buttonsState & BUTTON_SILENT_MASK) {
    systemState |= SYSTEM_SILENT_MASK;
  }

  if (telemetry.ldr.status &
      (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_VALID_ERR)) {
    systemState |= SYSTEM_LDR_ERR_MASK;
  }

  if (telemetry.dht.status &
      (STATUS_DHT_DEVICE_ERR | STATUS_DHT_DATA_VALID_ERR)) {
    systemState |= SYSTEM_DHT_ERR_MASK;
  }

  if (!isWifiConnected()) {
    systemState |= SYSTEM_WIFI_ERR_MASK;
  }

  if (!isMqttConnected()) {
    systemState |= SYSTEM_MQTT_ERR_MASK;
  }
}

static void handleWifiTest(const uint16_t &buttonsState) {
  if ((buttonsState & BUTTON_WIFI_DISABLE_MASK) &&
      !(previousButtonsState & BUTTON_WIFI_DISABLE_MASK)) {
    Serial.println("[TEST] WiFi disconnect");
    disconnectWiFi();
  }
}

static void updateLedState(const Telemetry &telemetry,
                           const uint16_t &systemState, uint16_t &ledState) {

  ledState = 0;

  if (systemState & SYSTEM_COMMAND_MASK) {
    ledState |= LED_COMMAND_MASK;
  }

  if (systemState & SYSTEM_SILENT_MASK) {
    ledState |= LED_SILENT_MASK;
  }

  updateLdrStatus(telemetry.ldr, ledState);
  updateDhtStatus(telemetry.dht, ledState);
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

static void handleCommand(const uint16_t &buttonsState) {

  if ((buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    setCommand(MANUAL_READ_COMMAND);
  }
}
