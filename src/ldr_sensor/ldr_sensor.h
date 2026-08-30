// src/ldr_sensor/ldr_sensor.h

#pragma once

#include "../telemetry/telemetry.h"
#include <Arduino.h>

#define STATUS_LDR_OK 0U

#define STATUS_LDR_DEVICE_ERR (1U << 0)
#define STATUS_LDR_DATA_VALID_ERR (1U << 1)

#define STATUS_LDR_LUX_ALARM_MIN (1U << 4)
#define STATUS_LDR_LUX_ALARM_MAX (1U << 5)

#define STATUS_LDR_INIT_ERR                                                    \
  (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_VALID_ERR |                         \
   STATUS_LDR_LUX_ALARM_MIN | STATUS_LDR_LUX_ALARM_MAX)

void initLdrSensor(LDRData &data);
void handleLdrSensor(LDRData &data, uint8_t &ledState);
