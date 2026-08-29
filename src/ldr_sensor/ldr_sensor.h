// src/ldr_sensor/ldr_sensor.h

#pragma once

#include "../telemetry/telemetry.h"
#include <Arduino.h>

#define STATUS_LDR_OK 0b00000000

#define STATUS_LDR_DEVICE_ERR 0b00000001
#define STATUS_LDR_DATA_VALID_ERR 0b00000010

#define STATUS_LDR_LUX_ALARM_MIN 0b00010000
#define STATUS_LDR_LUX_ALARM_MAX 0b00100000

#define STATUS_LDR_INIT_ERR                                                    \
  (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_VALID_ERR |                         \
   STATUS_LDR_LUX_ALARM_MIN | STATUS_LDR_LUX_ALARM_MAX)

void initLdrSensor(LDRData &data);
void handleLdrSensor(LDRData &data, uint8_t &ledState);
