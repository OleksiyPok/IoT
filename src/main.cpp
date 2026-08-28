// src/main.cpp

#include "bit_utils.h"
#include "buttons/buttons.h"
#include "config.h"
#include "dht_sensor/dht_sensor.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "monitor/monitor.h"
#include <Arduino.h>

unsigned char buttonsState = 0x00; // Button state register (8 buttons)
unsigned char ledState = 0x00;     // LED state register (8 leds)

unsigned long lastButtonsReadMs = 0;
unsigned long lastIndicationChangeMs = 0;
unsigned long lastLdrSensorReadMs = 0;
unsigned long lastDhtSensorReadMs = 0;
unsigned long lastDataMonitorMs = 0;
unsigned long lastMemoryCheckMs = 0;
unsigned long lastSendDataMs = 0;

SensorData currentSensorData;

// --------------

void setup() {
  initMonitor();
  initDhtSensor();
  // initLdrSensor();
  initButtons();
  initIndication();
}

void loop() {

  unsigned long now = millis();

  // DHT sensor reading
  if (now - lastDhtSensorReadMs >= SENSOR_DHT_READ_PERIOD_MS) {
    lastDhtSensorReadMs = now;

    handleDhtSensor(currentSensorData.dht);
    // dhtReadData(currentSensorData.dht);
  }

  // LDR sensor reading
  if (now - lastLdrSensorReadMs >= SENSOR_LDR_READ_PERIOD_MS) {
    lastLdrSensorReadMs = now;
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

    handleMonitor(currentSensorData);
  }

  // // Memory check
  // if (now - lastMemoryCheckMs >= MEMORY_CHECK_PERIOD_MS) {
  //   lastMemoryCheckMs = now;

  //   checkMemory();
  // }

  // delay(10); // To simplify the simulation process
}
