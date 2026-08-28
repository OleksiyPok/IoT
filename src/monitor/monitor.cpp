// src/monitor/monitor.cpp

#include "monitor.h"
#include "config.h"
#include "indication/indication.h"
#include <Arduino.h>

// ---------------------------------

// ---------------------------------

void initMonitor() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== Device started ===");
  Serial.println();
};

void handleMonitor(const Telemetry &data) {
  Serial.print("[DHT] Temperature: ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(data.dht.humidity, 1);
  Serial.print("%");
  Serial.print(" | (Status: 0b");
  for (int8_t i = 7; i >= 0; --i) {
    Serial.print((data.dht.status >> i) & 1);
  }
  Serial.println(")");

  Serial.print("[LDR] ADC: ");
  Serial.print(data.ldr.raw);
  Serial.print(", Lux: ");
  Serial.print(data.ldr.lux, 1);
  Serial.print(" | (Status: 0b");
  for (int8_t i = 7; i >= 0; --i) {
    Serial.print((data.ldr.status >> i) & 1);
  }
  Serial.println(")");
}
