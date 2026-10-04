// src/telemetry/telemetry.cpp

#include <Arduino.h>
#include <time.h>

#include "../device_info/device_info.h"
#include "../dht_sensor/dht_sensor.h"
#include "../ldr_sensor/ldr_sensor.h"
#include "../mqtt/mqtt_connection.h"
#include "../system/system_state.h"
#include "../time/time.h"
#include "../wifi/wifi.h"
#include "config.h"
#include "telemetry.h"

// ---------------------------------

void initTelemetry(Telemetry &telemetry) {
  telemetry.uptime = millis() / 1000;
  telemetry.version = TELEMETRY_PROTOCOL_VERSION;
  getDeviceId(telemetry.device_id);

  telemetry.buttonsState = 0x0000;
  telemetry.systemState = SYSTEM_STATE_ERR_INIT_MASK;
  telemetry.ledState = 0x0000;
}

void updateTelemetry(Telemetry &telemetry) {
  // Update timestamp
  telemetry.timestamp = getCurrentTimestamp();

  // Update "uptime"
  telemetry.uptime = millis() / 1000;
}

void updateStaleStatus(Telemetry &telemetry) {
  // Update ldr STALE status
  telemetry.ldr.status |= STATUS_LDR_DATA_STALE;
  // Update dht STALE status
  telemetry.dht.status |= STATUS_DHT_DATA_STALE;
}