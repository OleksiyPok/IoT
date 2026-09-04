// src/monitor/monitor_telemetry.cpp

#include <Arduino.h>

#include "monitor_telemetry.h"

// ---------------------------------

void printDeviceId(const Telemetry &data) {
  Serial.print("[TELEMETRY] DeviceId (MAC): ");

  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                (uint8_t)(data.deviceId >> 40), (uint8_t)(data.deviceId >> 32),
                (uint8_t)(data.deviceId >> 24), (uint8_t)(data.deviceId >> 16),
                (uint8_t)(data.deviceId >> 8), (uint8_t)data.deviceId);
}

void printTelemetryTimestamp(const Telemetry &data) {
  time_t timestamp = data.timestamp;

  struct tm timeInfo;
  gmtime_r(&timestamp, &timeInfo);

  Serial.printf(
      "[TELEMETRY] Timestamp: %04d-%02d-%02d %02d:%02d:%02d UTC (%lu)\r\n",
      timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
      timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
      static_cast<unsigned long>(data.timestamp));
}

void printTelemetryUptime(const Telemetry &data) {
  uint32_t hours = data.uptime / 3600;
  uint32_t minutes = (data.uptime % 3600) / 60;
  uint32_t seconds = data.uptime % 60;

  Serial.printf("[TELEMETRY] Uptime: %02lu:%02lu:%02lu (%lu sec)\r\n",
                static_cast<unsigned long>(hours),
                static_cast<unsigned long>(minutes),
                static_cast<unsigned long>(seconds),
                static_cast<unsigned long>(data.uptime));
}

void printTelemetrySequence(const Telemetry &data) {
  Serial.print("[TELEMETRY] Sequence: ");
  Serial.println(data.sequence);
}

// ---------------------------------

void printTelemetryData(const Telemetry &data) {
  printDeviceId(data);
  printTelemetryTimestamp(data);
  printTelemetryUptime(data);
  printTelemetrySequence(data);

  printLdrData(data);
  printDhtData(data);
}

// ---------------------------------

void printDhtData(const Telemetry &data) {
  Serial.print("[DHT] Temperature: ");
  Serial.print(data.dht.temperature, 1);
  Serial.print("°C,  Humidity: ");
  Serial.print(data.dht.humidity, 0);
  Serial.println("%");

  Serial.print("[DHT] Updated: ");
  Serial.println(data.dht.updated);
}

void printLdrData(const Telemetry &data) {
  Serial.print("[LDR] ADC: ");
  Serial.print(data.ldr.raw);
  Serial.print(", Lux: ");
  Serial.println(data.ldr.lux, 1);

  Serial.print("[LDR] Updated: ");
  Serial.println(data.ldr.updated);
}