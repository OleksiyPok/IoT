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
  telemetry.status |= STATUS_INIT_ERR;

  telemetry.buttonsStateN = 0x0000;
  telemetry.systemStateN = 0x0000;
  telemetry.ledState = 0x0000;
}

void updateTelemetry(Telemetry &telemetry, const uint16_t &systemState) {

  // Update timestamp
  telemetry.timestamp = getCurrentTimestamp();

  // Update "uptime"
  telemetry.uptime = millis() / 1000;

  // Update sequence
  telemetry.sequence = sequenceCounter++;

  telemetry.status &= ~(STATUS_DEVICE_SILENT_MODE | STATUS_LDR_ERR |
                        STATUS_DHT_ERR | STATUS_MQTT_ERR | STATUS_WIFI_ERR);

  if (systemState & SYSTEM_SILENT_MASK) {
    telemetry.status |= STATUS_DEVICE_SILENT_MODE;
  }

  if (systemState & SYSTEM_LDR_ERR_MASK) {
    telemetry.status |= STATUS_LDR_ERR;
  }

  if (systemState & SYSTEM_DHT_ERR_MASK) {
    telemetry.status |= STATUS_DHT_ERR;
  }

  if (systemState & SYSTEM_MQTT_ERR_MASK) {
    telemetry.status |= STATUS_MQTT_ERR;
  }

  if (systemState & SYSTEM_WIFI_ERR_MASK) {
    telemetry.status |= STATUS_WIFI_ERR;
  }
}
