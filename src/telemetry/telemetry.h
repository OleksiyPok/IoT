// src/telemetry/telemetry.h

#pragma once

#include <Arduino.h>

struct DHTData {
  float temperature; // °C
  float humidity;    // %
  uint8_t status;
};

struct LDRData {
  uint16_t raw; // ADC data (0–4095)
  float lux;    // data in the lux
  uint8_t status;
};

struct Telemetry {
  uint64_t deviceId;
  uint32_t timestamp;
  uint32_t uptime;
  DHTData dht;
  LDRData ldr;
  uint8_t status; // system status register
};

#define STATUS_OK 0U
#define STATUS_LDR_ERR (1U << 0)  // LDR error
#define STATUS_DHT_ERR (1U << 2)  // DHT22 error
#define STATUS_MQTT_ERR (1U << 6) // MQTT error
#define STATUS_WIFI_ERR (1U << 7) // Wi-Fi error

#define STATUS_INIT_ERR                                                        \
  (STATUS_LDR_ERR | STATUS_DHT_ERR | STATUS_MQTT_ERR | STATUS_WIFI_ERR)

void initTelemetry(Telemetry &telemetryData);
void updateTelemetry(Telemetry &telemetryData);