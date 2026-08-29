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
  uint32_t worktime;
  DHTData dht;
  LDRData ldr;
  uint8_t status; // system status register
};

#define STATUS_OK 0b00000000
#define STATUS_LDR_ERR 0b00000001  // bit 0: LDR error
#define STATUS_DHT_ERR 0b00000100  // bit 2: DHT22 error
#define STATUS_MQTT_ERR 0b01000000 // bit 6: MQTT error
#define STATUS_WIFI_ERR 0b10000000 // bit 7: Wi-Fi error

#define STATUS_INIT_ERR                                                        \
  (STATUS_LDR_ERR | STATUS_DHT_ERR | STATUS_MQTT_ERR | STATUS_WIFI_ERR)

void initTelemetry(Telemetry &telemetryData);
void updateTelemetry(Telemetry &telemetryData);