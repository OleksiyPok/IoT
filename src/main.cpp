// src/main.cpp

#include "config.h"
#include "dht_sensor/dht_sensor.h"
#include "indication/indication.h"
#include "ldr_sensor/ldr_sensor.h"
#include "monitor/monitor.h"
#include "sensors/sensors.h"

unsigned long lastSensorReadMs = 0;
unsigned long lastDataMonitorMs = 0;
unsigned long lastMemoryCheckMs = 0;

SensorData currentSensorData;

// --------------

void setup() {
  monitor_init();
  dht_sensor_init();
  ldr_sensor_init();
  indication_init();

  // delay(100);
}

void loop() {
  unsigned long now = millis();

  // Sensor reading
  if (now - lastSensorReadMs >= SENSOR_READ_PERIOD_MS) {
    lastSensorReadMs = now;
    blinkLed(LED_PIN);

    readSensor(&currentSensorData);
  }

  // Data monitor
  if (now - lastDataMonitorMs >= DATA_MONITOR_PERIOD_MS) {
    lastDataMonitorMs = now;
    blinkLed(LED_BUILTIN_PIN);

    monitorData(&currentSensorData);
  }

  // Memory check
  if (now - lastMemoryCheckMs >= MEMORY_CHECK_PERIOD_MS) {
    lastMemoryCheckMs = now;

    checkMemory();
  }

  delay(10); // To simplify the simulation process
}
