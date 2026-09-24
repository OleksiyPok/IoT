// src/main.cpp

#include <Arduino.h>
#include <time.h>

#include "actions/actions.h"
#include "buttons/buttons.h"
#include "commands/commands.h"
#include "config.h"
#include "dht_sensor/dht_sensor.h"
#include "http/http.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "memory/memory.h"
#include "monitor/monitor.h"
#include "mqtt/mqtt.h"
#include "mqtt/mqtt_connection.h"
#include "secrets.h"
#include "system/system_state.h"
#include "telemetry/telemetry.h"
#include "time/time.h"
#include "wifi/wifi.h"

// ---------------------------------

uint32_t lastWiFiCheckConnectionMs = 0;
uint32_t lastTimeSyncRequestMs = 0;
uint32_t lastButtonsReadMs = 0;
uint32_t lastActionsMs = 0;
uint32_t lastIndicationChangeMs = 0;
uint32_t lastStaleWatchdogMs = 0;
uint32_t lastLdrSensorReadMs = 0;
uint32_t lastDhtSensorReadMs = 0;
uint32_t lastDataMonitorMs = 0;
uint32_t lastMemoryCheckMs = 0;
uint32_t lastUpdateTelemetryMs = 0;
uint32_t lastSendDataMs = 0;
uint32_t lastMqttPublish = 0;

Telemetry telemetry;

// ---------------------------------
void setup() {
  initMonitor();
  initTelemetry(telemetry);
  initDhtSensor(telemetry.dht);
  initLdrSensor(telemetry.ldr);
  initButtons();
  initIndication();
  initWiFi();
}

void loop() {

  uint32_t now = millis();

  // WiFi check connection
  if (now - lastWiFiCheckConnectionMs >= WIFI_CHECK_INTERVAL_MS) {
    lastWiFiCheckConnectionMs = now;
    handleWiFi();
  }

  // System state
  updateSystemState(telemetry);

  // Connection sequence: Wi-Fi -> Time -> MQTT.
  if (telemetry.systemState & SYSTEM_WIFI_ERR_MASK) {
    lastTimeSyncRequestMs = 0;

  } else if (telemetry.systemState & SYSTEM_TIME_ERR_MASK) {
    if (lastTimeSyncRequestMs == 0 ||
        now - lastTimeSyncRequestMs >= TIME_SYNC_RETRY_INTERVAL_MS) {
      lastTimeSyncRequestMs = now;
      syncTime();
    }

  } else if (telemetry.systemState & SYSTEM_MQTT_ERR_MASK) {
    initMqtt();
  }

  // Buttons reading
  if (now - lastButtonsReadMs >= BUTTONS_READ_INTERVAL_MS) {
    lastButtonsReadMs = now;
    handleButtons(telemetry);
  }

  // STALE status watchdog
  if (now - lastStaleWatchdogMs >= SENSOR_STALE_INTERVAL_MS) {
    lastStaleWatchdogMs = now;
    updateStaleStatus(telemetry);
  }

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_INTERVAL_MS) {
    lastDhtSensorReadMs = now;
    if (!(telemetry.systemState & SYSTEM_SILENT_MASK)) {
      handleDhtSensor(telemetry.dht);
    }
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_INTERVAL_MS) {
    lastLdrSensorReadMs = now;
    handleLdrSensor(telemetry.ldr);
  }

  // Actions
  if (now - lastActionsMs >= ACTIONS_MS) {
    lastActionsMs = now;
    handleActions(telemetry);
  }

  // Telemetry update
  if (now - lastUpdateTelemetryMs >= TELEMETRY_UPDATE_INTERVAL_MS) {
    lastUpdateTelemetryMs = now;
    updateTelemetry(telemetry);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_INTERVAL_MS) {
    lastIndicationChangeMs = now;
    handleIndication();
  }

  // MQTT connection
  if (!(telemetry.systemState & SYSTEM_WIFI_ERR_MASK) &&
      !(telemetry.systemState & SYSTEM_TIME_ERR_MASK)) {
    handleMqttConnection(telemetry);
  }

  // MQTT publish
  if (!(telemetry.systemState & SYSTEM_WIFI_ERR_MASK) &&
      !(telemetry.systemState & SYSTEM_TIME_ERR_MASK) &&
      !(telemetry.systemState & SYSTEM_MQTT_ERR_MASK)) {

    handleMqttCommands();

    if ((now - lastMqttPublish) > MQTT_PUBLISH_INTERVAL_MS) {
      lastMqttPublish = now;
      handleMqttSensors(telemetry);
    }
  }

#if defined(DEBUG_MODE)
  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_INTERVAL_MS) {
    lastDataMonitorMs = now;
    handleMonitor(telemetry);
  }

  // Data send
  if (now - lastSendDataMs >= DATA_SEND_INTERVAL_MS) {
    lastSendDataMs = now;
    // handleSendData(telemetry);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_INTERVAL_MS) {
    lastMemoryCheckMs = now;
    // checkMemory();
  }
#endif

  delay(20); // To simplify the simulation process
}
