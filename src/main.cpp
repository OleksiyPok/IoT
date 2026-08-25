#include "config.h"
#include "monitor/monitor.h"
#include "sensors/sensors.h"

unsigned long lastSensorReadMs = 0;
unsigned long lastDataMonitorMs = 0;
unsigned long lastMemoryCheckMs = 0;

SensorData currentSensorData;

void blinkLed(int pin);

// --------------

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Device started ===");
  Serial.println();

  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);

  delay(500);
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

// --------------

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_ON_TIME_MS);
  digitalWrite(pin, LOW);
}
