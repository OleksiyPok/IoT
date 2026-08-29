// src/main.cpp

#include "main.h"
#include "buttons/buttons.h"
#include "config.h"
#include "device_info/device_info.h"
#include "dht_sensor/dht_sensor.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "memory/memory.h"
#include "monitor/monitor.h"
#include "wifi/wifi.h"
#include <Arduino.h>

// ---------------------------------

uint32_t lastButtonsReadMs = 0;
uint32_t lastIndicationChangeMs = 0;
uint32_t lastLdrSensorReadMs = 0;
uint32_t lastDhtSensorReadMs = 0;
uint32_t lastDataMonitorMs = 0;
uint32_t lastMemoryCheckMs = 0;
uint32_t lastSendDataMs = 0;

Telemetry telemetryData;
void deviceInfo();

// ---------------------------------

void setup() {
  initMonitor();
  connectWifi();
  deviceInfo();
  initDhtSensor();
  initLdrSensor();
  initButtons();
  initIndication();
}

void loop() {

  uint32_t now = millis();

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_PERIOD_MS) {
    lastDhtSensorReadMs = now;
    // handleDhtSensor(telemetryData.dht, ledState);
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_PERIOD_MS) {
    lastLdrSensorReadMs = now;
    // handleLdrSensor(telemetryData.ldr, ledState);
  }

  // Buttons reading
  if (now - lastButtonsReadMs >= BUTTONS_READ_PERIOD_MS) {
    lastButtonsReadMs = now;
    // handleButtons(ledState);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_PERIOD_MS) {
    lastIndicationChangeMs = now;
    // handleIndication(ledState);
  }

  // Data send
  if (now - lastSendDataMs >= SEND_DATA_PERIOD_MS) {
    lastSendDataMs = now;
  }

  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_PERIOD_MS) {
    lastDataMonitorMs = now;
    handleMonitor(telemetryData);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_PERIOD_MS) {
    lastMemoryCheckMs = now;
    // checkMemory();
  }

  delay(50); // To simplify the simulation process
}

void deviceInfo() {
  if (!getDeviceId(telemetryData.deviceId)) {
    Serial.println("Failed to get device ID");
  }
};