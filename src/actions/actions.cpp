// src/actions/actions.cpp

#include "actions.h"
#include "../buttons/buttons.h"
#include "../commands/commands.h"
#include "../config.h"
#include "../indication/indication.h"
#include "../mqtt/mqtt_connection.h"
#include "../mqtt/mqtt_subscribe.h"
#include "../system/system_state.h"
#include "../wifi/wifi.h"

// ---------------------------------
static uint16_t previousButtonsState = 0x0000;

static void handleWifiTest(const uint16_t &buttonsState);
static void handleCommand(const uint16_t &buttonsState);
static void updateMqttSensorStatus(const MqttSensorData &data,
                                   uint16_t &ledState);
static void updateLedState(const uint16_t &systemState, uint16_t &ledState);
static void updateSystemState(const uint16_t &buttonsState,
                              uint16_t &systemState);

static void handleMqttCommand();

static bool mqttTemperatureMaxActive = false;
static bool mqttLightAutoActive = false;

// ---------------------------------
void handleActions(const uint16_t &buttonsState, uint16_t &systemState,
                   uint16_t &ledState) {

  updateSystemState(buttonsState, systemState);
  updateLedState(systemState, ledState);
  handleCommand(buttonsState);
  handleMqttCommand();
  handleWifiTest(buttonsState);

  previousButtonsState = buttonsState;
}

static void updateSystemState(const uint16_t &buttonsState,
                              uint16_t &systemState) {

  systemState &= ~SYSTEM_STATE_MANAGED_MASK;

  if ((buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    systemState |= SYSTEM_COMMAND_MASK;
  }

  if (buttonsState & BUTTON_SILENT_MASK) {
    systemState |= SYSTEM_SILENT_MASK;
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

static void updateLedState(const uint16_t &systemState, uint16_t &ledState) {

  ledState = 0;

  if (systemState & SYSTEM_COMMAND_MASK) {
    ledState |= LED_COMMAND_MASK;
  }

  if (systemState & SYSTEM_SILENT_MASK) {
    ledState |= LED_SILENT_MASK;
  }

  updateMqttSensorStatus(getMqttSensorData(), ledState);
}

static void handleCommand(const uint16_t &buttonsState) {

  if ((buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    setCommand(MANUAL_READ_COMMAND);
  }
}

static void handleMqttCommand() {
  const char *command = getReceivedCommand();

  if (command == nullptr) {
    return;
  }

  if (strcmp(command, MANUAL_READ_COMMAND) == 0) {
    Serial.println(MANUAL_READ_COMMAND);
    blinkLed(LED_COMMAND_PIN);
  }

  clearReceivedCommand();
}

static void updateMqttSensorStatus(const MqttSensorData &data,
                                   uint16_t &ledState) {
  // Temperature hysteresis:
  // > 26 C -> ON
  // < 20 C -> OFF
  // 20..26 C -> keep previous state.
  if (data.hasTemperature) {
    if (data.temperature > DHT_TEMPERATURE_ALARM_MAX_CONFIG) {
      mqttTemperatureMaxActive = true;
    } else if (data.temperature < DHT_TEMPERATURE_ALARM_MIN_CONFIG) {
      mqttTemperatureMaxActive = false;
    }
  }

  // Light:
  // < 600 lux -> ON
  // > 600 lux -> OFF
  // 600 lux -> keep previous state.
  if (data.hasLdrLux) {
    if (data.ldrLux < LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG) {
      mqttLightAutoActive = true;
    } else if (data.ldrLux > LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG) {
      mqttLightAutoActive = false;
    }
  }

  if (data.hasTemperature) {
    ledState &= ~LED_TEMPERATURE_MAX_MASK;

    if (mqttTemperatureMaxActive) {
      ledState |= LED_TEMPERATURE_MAX_MASK;
    }
  }

  if (data.hasLdrLux) {
    ledState &= ~LED_LIGHT_AUTO_MASK;

    if (mqttLightAutoActive) {
      ledState |= LED_LIGHT_AUTO_MASK;
    }
  }
}
