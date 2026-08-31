// src/main.cpp

#include "actions/actions.h"
#include "buttons/buttons.h"
#include "config.h"
#include "dht_sensor/dht_sensor.h"
#include "http/http.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "memory/memory.h"
#include "monitor/monitor.h"
#include "telemetry/telemetry.h"
#include "wifi/wifi.h"
#include <Arduino.h>

// ---------------------------------

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

Telemetry telemetryData;

// ---------------------------------

void setup() {
  initMonitor();
  initTelemetry(telemetryData);
  initDhtSensor(telemetryData.dht);
  initLdrSensor(telemetryData.ldr);
  initButtons();
  initIndication();
  connectWifi();
}

void loop() {

  uint32_t now = millis();

  // WiFi check connection
  if (now - lastWiFiCheckConnectionMs >= WIFI_CHECK_INTERVAL_MS) {
    lastWiFiCheckConnectionMs = now;
    handleWiFi();
  }

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_INTERVAL_MS) {
    lastDhtSensorReadMs = now;
    handleDhtSensor(telemetryData.dht);
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_INTERVAL_MS) {
    lastLdrSensorReadMs = now;
    handleLdrSensor(telemetryData.ldr);
  }

  // Buttons reading
  if (now - lastButtonsReadMs >= BUTTONS_READ_INTERVAL_MS) {
    lastButtonsReadMs = now;
    handleButtons();
  }

  // Actions
  if (now - lastActionsMs >= ACTIONS_MS) {
    lastActionsMs = now;
    handleActions(telemetryData, buttonsState, ledState);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_INTERVAL_MS) {
    lastIndicationChangeMs = now;
    handleIndication(ledState);
  }

  // Telemetry update
  if (now - lastUpdateTelemetryMs >= TELEMETRY_UPDATE_INTERVAL_MS) {
    lastUpdateTelemetryMs = now;
    updateTelemetry(telemetryData);
  }

  // Data send
  if (now - lastSendDataMs >= DATA_SEND_INTERVAL_MS) {
    lastSendDataMs = now;
    // handleSendData(telemetryData);
  }

  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_INTERVAL_MS) {
    lastDataMonitorMs = now;

    if (buttonsState & BUTTON_SERIAL_MONITOR_MASK) {
      handleMonitor(telemetryData);
    }
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_INTERVAL_MS) {
    lastMemoryCheckMs = now;
    // checkMemory();
  }

  delay(20); // To simplify the simulation process
}
