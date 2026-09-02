// src/telemetry/telemetry.cpp

#include <Arduino.h>

#include "../buttons/buttons.h"
#include "../device_info/device_info.h"
#include "../dht_sensor/dht_sensor.h"
#include "../indication/indication.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../wifi/wifi.h"
#include "config.h"
#include "telemetry.h"

// ---------------------------------

// ---------------------------------

void initTelemetry(Telemetry &telemetryData) {
  telemetryData.uptime = millis() / 1000;
  getDeviceId(telemetryData.deviceId);
  telemetryData.status |= STATUS_INIT_ERR;
}

void updateTelemetry(Telemetry &telemetryData, uint8_t systemState) {
  // Update "uptime"
  uint32_t now = millis();
  telemetryData.uptime = millis() / 1000;

  // Update SILENT system status.
  if (systemState & LED_SILENT_MASK) {
    telemetryData.status |= STATUS_DEVICE_SILENT_MODE;
  } else {
    telemetryData.status &= ~STATUS_DEVICE_SILENT_MODE;
  }

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

  // Update STALE status.
  if (now - telemetryData.dht.updated > (SENSOR_DHT_READ_INTERVAL_MS)) {
    telemetryData.dht.status |= STATUS_DHT_DATA_STALE;
  } else {
    telemetryData.dht.status &= ~STATUS_DHT_DATA_STALE;
  }

  if (now - telemetryData.ldr.updated > (SENSOR_LDR_READ_INTERVAL_MS)) {
    telemetryData.ldr.status |= STATUS_LDR_DATA_STALE;
  } else {
    telemetryData.ldr.status &= ~STATUS_LDR_DATA_STALE;
  }

  // Update WiFi system status.
  if (isWifiConnected()) {
    telemetryData.status &= ~STATUS_WIFI_ERR;
  } else {
    telemetryData.status |= STATUS_WIFI_ERR;
  }
}