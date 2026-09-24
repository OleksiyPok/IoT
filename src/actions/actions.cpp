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
#include "config.h"

// ---------------------------------
static uint16_t previousButtonsState = 0x0000;

static void updateLedIndications(const Telemetry &telemetry);
static void handleWifiTest(const Telemetry &telemetry);
static void handleCommand(const Telemetry &telemetry);

// ---------------------------------
void handleActions(Telemetry &telemetry) {

  handleCommand(telemetry);
  handleWifiTest(telemetry);
  updateLedIndications(telemetry);

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

static void updateLedIndications(const Telemetry &telemetry) {

  requestIndication(LED_COMMAND_PIN, telemetry.systemState & SYSTEM_COMMAND_MASK
                                         ? IndicationType::ON
                                         : IndicationType::OFF);

  requestIndication(LED_SILENT_PIN, telemetry.systemState & SYSTEM_SILENT_MASK
                                        ? IndicationType::ON
                                        : IndicationType::OFF);

  requestIndication(LED_LIGHT_MIN_PIN,
                    telemetry.ldr.status & STATUS_LDR_LUX_ALARM_MIN
                        ? IndicationType::ON
                        : IndicationType::OFF);

  requestIndication(LED_LIGHT_MAX_PIN,
                    telemetry.ldr.status & STATUS_LDR_LUX_ALARM_MAX
                        ? IndicationType::ON
                        : IndicationType::OFF);

  requestIndication(LED_LIGHT_AUTO_PIN,
                    telemetry.ldr.status & STATUS_LDR_LIGHT_LOW
                        ? IndicationType::ON
                        : IndicationType::OFF);

  requestIndication(LED_TEMPERATURE_MIN_PIN,
                    telemetry.dht.status & STATUS_DHT_TEMPERATURE_ALARM_MIN
                        ? IndicationType::ON
                        : IndicationType::OFF);

  requestIndication(LED_TEMPERATURE_MAX_PIN,
                    telemetry.dht.status & STATUS_DHT_TEMPERATURE_ALARM_MAX
                        ? IndicationType::ON
                        : IndicationType::OFF);

  requestIndication(LED_HUMIDITY_MIN_PIN,
                    telemetry.dht.status & STATUS_DHT_HUMIDITY_ALARM_MIN
                        ? IndicationType::ON
                        : IndicationType::OFF);

  // requestIndication(LED_HUMIDITY_MAX_PIN,
  //                   telemetry.dht.status & STATUS_DHT_HUMIDITY_ALARM_MAX
  //                       ? IndicationType::ON
  //                       : IndicationType::OFF);
}