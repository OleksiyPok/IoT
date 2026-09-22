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
static uint8_t sequenceCounter = 0;

static uint8_t dhtStaleCycles = 0;
static uint8_t ldrStaleCycles = 0;

static uint32_t previousDhtUptime = 0;
static uint32_t previousLdrUptime = 0;

// ---------------------------------
void initTelemetry(Telemetry &telemetry) {
  telemetry.uptime = millis() / 1000;
  telemetry.version = TELEMETRY_PROTOCOL_VERSION;
  getDeviceId(telemetry.deviceId);

  telemetry.systemState &= ~SYSTEM_STATE_MANAGED_MASK;
  telemetry.systemState |= SYSTEM_STATE_ERR_INIT_MASK;

  telemetry.buttonsState = 0x0000;
  telemetry.systemState = 0x0000;
  telemetry.ledState = 0x0000;
}

void updateTelemetry(Telemetry &telemetry) {
  // Update timestamp
  telemetry.timestamp = getCurrentTimestamp();

  // Update "uptime"
  telemetry.uptime = millis() / 1000;

  // Update sequence
  telemetry.sequence = sequenceCounter++;
}

void updateStaleStatus(Telemetry &telemetry) {
  // Update ldr STALE status
  telemetry.ldr.status |= STATUS_LDR_DATA_STALE;
  // Update dht STALE status
  telemetry.dht.status |= STATUS_DHT_DATA_STALE;
}