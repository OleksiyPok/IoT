// src/types.h

#pragma once

#include "dht_sensor/dht_sensor.h"
#include "ldr_sensor/ldr_sensor.h"
#include <Arduino.h>

struct Telemetry {
  uint64_t deviceId;
  uint32_t worktime;
  DHTData dht;
  LDRData ldr;
  uint8_t status; // system status register
};