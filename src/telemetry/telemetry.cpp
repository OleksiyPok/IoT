// src/telemetry/telemetry.cpp

#include "telemetry.h"
#include "../dht_sensor/dht_sensor.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../wifi/wifi.h"
#include "device_info/device_info.h"
#include <Arduino.h>

// ---------------------------------

// ---------------------------------
void initTelemetry(Telemetry &telemetryData) {
  telemetryData.worktime = millis() / 1000;
  getDeviceId(telemetryData.deviceId);
  telemetryData.status |= STATUS_INIT_ERR;
}

void updateTelemetry(Telemetry &telemetryData) {
  telemetryData.worktime = millis() / 1000;

  // Update DHT system status
  if (telemetryData.dht.status &
      (STATUS_DHT_DEVICE_ERR | STATUS_DHT_DATA_VALID_ERR)) {
    telemetryData.status |= STATUS_DHT_ERR;
  } else {
    telemetryData.status &= ~STATUS_DHT_ERR;
  }

  // Update LDR system status.
  if (telemetryData.ldr.status &
      (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_VALID_ERR)) {
    telemetryData.status |= STATUS_LDR_ERR;
  } else {
    telemetryData.status &= ~STATUS_LDR_ERR;
  }

  // Update WiFi system status.
  if (isWifiConnected()) {
    telemetryData.status &= ~STATUS_WIFI_ERR;
  } else {
    telemetryData.status |= STATUS_WIFI_ERR;
    connectWifi();
  }
}