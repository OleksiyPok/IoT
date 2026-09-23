// src/system/system_state.cpp

#include "system_state.h"
#include "../buttons/buttons.h"
#include "../dht_sensor/dht_sensor.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../mqtt/mqtt_connection.h"
#include "../wifi/wifi.h"

// ---------------------------------

static uint16_t previousButtonsState = 0x0000;

// ---------------------------------

void updateSystemState(Telemetry &telemetry) {
  // All errors are active until confirmed otherwise.
  telemetry.systemState |= SYSTEM_STATE_ERR_INIT_MASK;
  // Managed states.
  telemetry.systemState &= ~SYSTEM_STATE_MANAGED_MASK;

  if ((telemetry.buttonsState & BUTTON_COMMAND_MASK) &&
      !(previousButtonsState & BUTTON_COMMAND_MASK)) {
    telemetry.systemState |= SYSTEM_COMMAND_MASK;
  }

  if (telemetry.buttonsState & BUTTON_SILENT_MASK) {
    telemetry.systemState |= SYSTEM_SILENT_MASK;
  }

  // LDR error confirmation.
  if (!(telemetry.ldr.status &
        (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_VALID_ERR))) {
    telemetry.systemState &= ~SYSTEM_LDR_ERR_MASK;
  }

  // DHT error confirmation.
  if (!(telemetry.dht.status &
        (STATUS_DHT_DEVICE_ERR | STATUS_DHT_DATA_VALID_ERR))) {
    telemetry.systemState &= ~SYSTEM_DHT_ERR_MASK;
  }

  // Wi-Fi error confirmation.
  if (isWifiConnected()) {
    telemetry.systemState &= ~SYSTEM_WIFI_ERR_MASK;
  }

  // MQTT error confirmation.
  if (isMqttConnected()) {
    telemetry.systemState &= ~SYSTEM_MQTT_ERR_MASK;
  }

  previousButtonsState = telemetry.buttonsState;
}