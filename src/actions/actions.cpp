// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../commands/commands.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../mqtt/mqtt.h"
#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"

// ---------------------------------
static uint16_t previousButtonsState = 0x0000;

static void updateSystemState(const Telemetry &telemetry,
                              uint16_t &systemState);
static void updateDhtStatus(const DHTData &data, Telemetry &telemetry);
static void updateLdrStatus(const LDRData &data, Telemetry &telemetry);
static void updateLedState(Telemetry &telemetry, const uint16_t &systemState);

static void handleWifiTest(const Telemetry &telemetry);
static void handleCommand(const Telemetry &telemetry);

// ---------------------------------
void handleActions(Telemetry &telemetry, uint16_t &systemState) {

  updateSystemState(telemetry, systemState);
  updateLedState(telemetry, systemState);
  handleCommand(telemetry);
  handleWifiTest(telemetry);

  previousButtonsState = telemetry.buttonsState;
}

static void updateSystemState(const Telemetry &telemetry,
                              uint16_t &systemState) {

  systemState &= ~SYSTEM_STATE_MANAGED_MASK;

  if ((telemetry.buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    systemState |= SYSTEM_COMMAND_MASK;
  }

  if (telemetry.buttonsState & BUTTON_SILENT_MASK) {
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

static void handleWifiTest(const Telemetry &telemetry) {
  if ((telemetry.buttonsState & BUTTON_WIFI_DISABLE_MASK) &&
      !(previousButtonsState & BUTTON_WIFI_DISABLE_MASK)) {
    Serial.println("[TEST] WiFi disconnect");
    disconnectWiFi();
  }
}

static void updateLedState(Telemetry &telemetry, const uint16_t &systemState) {

  telemetry.ledState = 0;

  if (systemState & SYSTEM_COMMAND_MASK) {
    telemetry.ledState |= LED_COMMAND_MASK;
  }

  if (systemState & SYSTEM_SILENT_MASK) {
    telemetry.ledState |= LED_SILENT_MASK;
  }

  updateLdrStatus(telemetry.ldr, telemetry);
  updateDhtStatus(telemetry.dht, telemetry);
}

static void updateDhtStatus(const DHTData &data, Telemetry &telemetry) {
  telemetry.ledState &= ~(LED_TEMPERATURE_MIN_MASK | LED_TEMPERATURE_MAX_MASK |
                          LED_HUMIDITY_MIN_MASK);

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MIN) {
    telemetry.ledState |= LED_TEMPERATURE_MIN_MASK;
  }

  if (data.status & STATUS_DHT_TEMPERATURE_ALARM_MAX) {
    telemetry.ledState |= LED_TEMPERATURE_MAX_MASK;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MIN) {
    telemetry.ledState |= LED_HUMIDITY_MIN_MASK;
  }

  if (data.status & STATUS_DHT_HUMIDITY_ALARM_MAX) {
    telemetry.ledState |= LED_HUMIDITY_MAX_MASK;
  }
}

static void updateLdrStatus(const LDRData &data, Telemetry &telemetry) {
  telemetry.ledState &=
      ~(LED_LIGHT_MIN_MASK | LED_LIGHT_MAX_MASK | LED_LIGHT_AUTO_MASK);

  if (data.status & STATUS_LDR_LUX_ALARM_MIN) {
    telemetry.ledState |= LED_LIGHT_MIN_MASK;
  }

  if (data.status & STATUS_LDR_LUX_ALARM_MAX) {
    telemetry.ledState |= LED_LIGHT_MAX_MASK;
  }

  if (data.status & STATUS_LDR_LIGHT_LOW) {
    telemetry.ledState |= LED_LIGHT_AUTO_MASK;
  }
}

static void handleCommand(const Telemetry &telemetry) {

  if ((telemetry.buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    setCommand(MANUAL_READ_COMMAND);
  }
}
