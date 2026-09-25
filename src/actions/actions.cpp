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

static void updateLedState(Telemetry &telemetry);
static void updateDhtLedState(Telemetry &telemetry);
static void updateLdrLedState(Telemetry &telemetry);

static void handleWifiTest(const Telemetry &telemetry);
static void handleCommand(const Telemetry &telemetry);

// ---------------------------------
void handleActions(Telemetry &telemetry) {

  handleCommand(telemetry);
  handleWifiTest(telemetry);
  updateLedState(telemetry);

  previousButtonsState = telemetry.buttonsState;
}

static void handleWifiTest(const Telemetry &telemetry) {
  if ((telemetry.buttonsState & BUTTON_WIFI_DISABLE_MASK) &&
      !(previousButtonsState & BUTTON_WIFI_DISABLE_MASK)) {
    Serial.println("[TEST] WiFi disconnect");
    disconnectWiFi();
  }
}

static void handleCommand(const Telemetry &telemetry) {

  if ((telemetry.buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    setCommand(MANUAL_READ_COMMAND);
  }
}

static void updateLedState(Telemetry &telemetry) {

  if (telemetry.systemState & SYSTEM_COMMAND_MASK) {
    telemetry.ledState |= LED_COMMAND_MASK;
  } else {
    telemetry.ledState &= ~LED_COMMAND_MASK;
  }

  if (telemetry.systemState & SYSTEM_SILENT_MASK) {
    telemetry.ledState |= LED_SILENT_MASK;
  } else {
    telemetry.ledState &= ~LED_SILENT_MASK;
  }

  updateLdrLedState(telemetry);
  updateDhtLedState(telemetry);
}

static void updateLdrLedState(Telemetry &telemetry) {
  telemetry.ledState &=
      ~(LED_LIGHT_MIN_MASK | LED_LIGHT_MAX_MASK | LED_LIGHT_AUTO_MASK);

  if (telemetry.ldr.status & STATUS_LDR_LUX_ALARM_MIN) {
    telemetry.ledState |= LED_LIGHT_MIN_MASK;
  }

  if (telemetry.ldr.status & STATUS_LDR_LUX_ALARM_MAX) {
    telemetry.ledState |= LED_LIGHT_MAX_MASK;
  }

  if (telemetry.ldr.status & STATUS_LDR_LIGHT_LOW) {
    telemetry.ledState |= LED_LIGHT_AUTO_MASK;
  }
}

static void updateDhtLedState(Telemetry &telemetry) {
  telemetry.ledState &= ~(LED_TEMPERATURE_MIN_MASK | LED_TEMPERATURE_MAX_MASK |
                          LED_HUMIDITY_MIN_MASK | LED_HUMIDITY_MAX_MASK);

  if (telemetry.dht.status & STATUS_DHT_TEMPERATURE_ALARM_MIN) {
    telemetry.ledState |= LED_TEMPERATURE_MIN_MASK;
  }

  if (telemetry.dht.status & STATUS_DHT_TEMPERATURE_ALARM_MAX) {
    telemetry.ledState |= LED_TEMPERATURE_MAX_MASK;
  }

  if (telemetry.dht.status & STATUS_DHT_HUMIDITY_ALARM_MIN) {
    telemetry.ledState |= LED_HUMIDITY_MIN_MASK;
  }

  if (telemetry.dht.status & STATUS_DHT_HUMIDITY_ALARM_MAX) {
    telemetry.ledState |= LED_HUMIDITY_MAX_MASK;
  }
}
