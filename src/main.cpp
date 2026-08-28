// src/main.cpp

#include "bit_utils.h"
#include "buttons/buttons.h"
#include "config.h"
#include "dht_sensor/dht_sensor.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "memory/memory.h"
#include "monitor/monitor.h"
#include <Arduino.h>

// ---------------------------------

uint8_t buttonsState = 0x00; // Button state register (8 buttons)
uint8_t ledState = 0x00;     // LED state register (8 leds)

uint32_t lastButtonsReadMs = 0;
uint32_t lastIndicationChangeMs = 0;
uint32_t lastLdrSensorReadMs = 0;
uint32_t lastDhtSensorReadMs = 0;
uint32_t lastDataMonitorMs = 0;
uint32_t lastMemoryCheckMs = 0;
uint32_t lastSendDataMs = 0;

Telemetry telemetryData;

// ---------------------------------

void setup() {
  initMonitor();
  initDhtSensor();
  // initLdrSensor();
  initButtons();
  initIndication();
}

void loop() {

  uint32_t now = millis();

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_PERIOD_MS) {
    lastDhtSensorReadMs = now;

    handleDhtSensor(telemetryData.dht);
    // dhtReadData(telemetryData.dht);
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_PERIOD_MS) {
    lastLdrSensorReadMs = now;

    handleLdrSensor(telemetryData.ldr);
  }

  // Buttons reading
  if (now - lastButtonsReadMs >= BUTTONS_READ_PERIOD_MS) {
    lastButtonsReadMs = now;

    handleButtons(buttonsState);
  }

  // Indication
  if (now - lastIndicationChangeMs >= INDICATION_CHANGE_PERIOD_MS) {
    lastIndicationChangeMs = now;

    ledState = buttonsState;
    handleIndication(ledState);
  }

  // Data send
  if (now - lastSendDataMs >= SEND_DATA_PERIOD_MS) {
  }

  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_PERIOD_MS) {
    lastDataMonitorMs = now;

    // Serial.print("buttonsState = ");
    // Serial.print(buttonsState);
    // Serial.print("  ");
    // Serial.print("ledState = ");
    // Serial.println(ledState);

    handleMonitor(telemetryData);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_PERIOD_MS) {
    lastMemoryCheckMs = now;

    checkMemory();
  }

  // delay(10); // To simplify the simulation process
}
