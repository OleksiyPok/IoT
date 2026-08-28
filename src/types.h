// src/types.h

#pragma once

#include "dht_sensor/dht_sensor.h"
#include "ldr_sensor/ldr_sensor.h"
#include <Arduino.h>

struct Telemetry {
  uint64_t deviceId;
  DHTData dht;
  LDRData ldr;
  uint32_t worktime;
  uint8_t status; // system status register
};