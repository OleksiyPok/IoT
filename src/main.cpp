// src/main.cpp

#include <Arduino.h>
#include <time.h>

#include "actions/actions.h"
#include "buttons/buttons.h"
#include "commands/commands.h"
#include "config.h"
#include "indication/indication.h"
#include "memory/memory.h"
#include "monitor/monitor.h"
#include "mqtt/mqtt_connection.h"
#include "system/system_state.h"
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
uint32_t lastDataMonitorMs = 0;
uint32_t lastMemoryCheckMs = 0;

// ---------------------------------

void setup() {
  initMonitor();
  initButtons();
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

  // Actions
  if (now - lastActionsMs >= ACTIONS_MS) {
    lastActionsMs = now;
    handleActions(buttonsState, systemState, ledState);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_INTERVAL_MS) {
    lastIndicationChangeMs = now;
    handleIndication(ledState);
  }

  // MQTT connection
  handleMqttConnection();

#if defined(DEBUG_MODE)
  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_INTERVAL_MS) {
    lastDataMonitorMs = now;
    handleMonitor(systemState, buttonsState, ledState);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_INTERVAL_MS) {
    lastMemoryCheckMs = now;
    // checkMemory();
  }
#endif

  delay(20); // To simplify the simulation process
}
