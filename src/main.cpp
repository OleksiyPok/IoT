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
#include "system/system_state.h"
#include "telemetry/telemetry.h"
#include "time/time.h"
#include "wifi/wifi.h"

// ---------------------------------

uint16_t buttonsState = 0x0000;
uint16_t systemState = 0x0000;
uint16_t ledState = 0x0000;

uint32_t lastWiFiCheckConnectionMs = 0;
uint32_t lastButtonsReadMs = 0;
uint32_t lastActionsMs = 0;
uint32_t lastIndicationChangeMs = 0;
uint32_t lastLdrSensorReadMs = 0;
uint32_t lastDhtSensorReadMs = 0;
uint32_t lastDataMonitorMs = 0;
uint32_t lastMemoryCheckMs = 0;
uint32_t lastUpdateTelemetryMs = 0;
uint32_t lastSendDataMs = 0;
uint32_t lastMqttPublish = 0;

Telemetry telemetryData;
CommandQueue commandQueue;

// ---------------------------------

void setup() {
  initMonitor();
  initTelemetry(telemetryData);
  initDhtSensor(telemetryData.dht);
  initLdrSensor(telemetryData.ldr);
  initButtons();
  initCommandQueue(commandQueue);
  initIndication();
  connectWifi();
  initTime();
  initMqtt();
}

void loop() {

  uint32_t now = millis();

  // WiFi check connection
  if (now - lastWiFiCheckConnectionMs >= WIFI_CHECK_INTERVAL_MS) {
    lastWiFiCheckConnectionMs = now;
    handleWiFi();
  }

  // Buttons reading
  if (now - lastButtonsReadMs >= BUTTONS_READ_INTERVAL_MS) {
    lastButtonsReadMs = now;
    handleButtons(buttonsState);
  }

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_INTERVAL_MS) {
    lastDhtSensorReadMs = now;
    if (!(systemState & SYSTEM_SILENT_MASK)) {
      handleDhtSensor(telemetryData.dht);
    }
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_INTERVAL_MS) {
    lastLdrSensorReadMs = now;
    handleLdrSensor(telemetryData.ldr);
  }

  // Actions
  if (now - lastActionsMs >= ACTIONS_MS) {
    lastActionsMs = now;
    handleActions(telemetryData, buttonsState, systemState, ledState,
                  commandQueue);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_INTERVAL_MS) {
    lastIndicationChangeMs = now;
    handleIndication(ledState);
  }

  // Telemetry update
  if (now - lastUpdateTelemetryMs >= TELEMETRY_UPDATE_INTERVAL_MS) {
    lastUpdateTelemetryMs = now;
    updateTelemetry(telemetryData, systemState);
  }

  // MQTT connection
  handleMqttConnection();

  // MQTT publish
  if ((now - lastMqttPublish) > MQTT_PUBLISH_INTERVAL_MS) {
    lastMqttPublish = now;
    handleMqtt(telemetryData, commandQueue);
  }

#if defined(DEBUG_MODE)
  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_INTERVAL_MS) {
    lastDataMonitorMs = now;
    handleMonitor(telemetryData, systemState, buttonsState, ledState);
  }

  // Data send
  if (now - lastSendDataMs >= DATA_SEND_INTERVAL_MS) {
    lastSendDataMs = now;
    // handleSendData(telemetryData);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_INTERVAL_MS) {
    lastMemoryCheckMs = now;
    // checkMemory();
  }
#endif

  delay(20); // To simplify the simulation process
}
