// src/telemetry/telemetry.h

#pragma once

#include <Arduino.h>

// ---------------------------------

// ---------------------------------
struct DHTData {
  float temperature; // °C
  float humidity;    // %
  uint32_t updated;
  uint32_t uptime;
  uint16_t status;
};

struct LDRData {
  uint16_t raw; // ADC data (0–4095)
  float lux;    // data in the lux
  uint32_t updated;
  uint32_t uptime;
  uint16_t status;
};

struct Telemetry {
  uint8_t version; // protocol version
  uint64_t deviceId;
  uint32_t timestamp;
  uint32_t uptime;
  uint8_t sequence;
  DHTData dht;
  LDRData ldr;
  uint16_t status; // system status register

  uint16_t buttonsState;
  uint16_t systemState;
  uint16_t ledState;
};

// struct System {
//   uint16_t buttonsState;
//   uint16_t systemState;
//   uint16_t ledState;
// }

#define TELEMETRY_PROTOCOL_VERSION 3

void initTelemetry(Telemetry &telemetry);
void updateTelemetry(Telemetry &telemetry);
void updateStaleStatus(Telemetry &telemetry);
