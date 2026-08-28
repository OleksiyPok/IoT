// src/monitor/monitor.cpp

// // src/monitor/monitor.cpp

#include "monitor.h"
#include "config.h"
#include "indication/indication.h"
#include <Arduino.h>

void initMonitor() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Device started ===");
  Serial.println();
};

void handleMonitor(const SensorData &data) {
  Serial.print("[DHT] ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("C  ");
  Serial.print(data.dht.humidity, 1);
  Serial.println("%");

  void checkMemory();
}

void checkMemory() {
  Serial.println("------ Free heap ------");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.println("-----------------------");
  Serial.println();
}
