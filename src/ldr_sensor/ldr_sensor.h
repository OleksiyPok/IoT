// src/ldr_sensor/ldr_sensor.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

#define STATUS_LDR_OK (0U)

#define STATUS_LDR_DEVICE_ERR (1U << 0)
#define STATUS_LDR_DATA_STALE (1U << 1)
#define STATUS_LDR_DATA_VALID_ERR (1U << 2)

#define STATUS_LDR_LUX_ALARM_MIN (1U << 4)
#define STATUS_LDR_LUX_ALARM_MAX (1U << 5)
#define STATUS_LDR_LIGHT_LOW (1U << 6)

#define STATUS_LDR_INIT_ERR                                                    \
  (STATUS_LDR_DEVICE_ERR | STATUS_LDR_DATA_STALE | STATUS_LDR_DATA_VALID_ERR | \
   STATUS_LDR_LUX_ALARM_MIN | STATUS_LDR_LUX_ALARM_MAX | STATUS_LDR_LIGHT_LOW)

void initLdrSensor(LDRData &data);
void handleLdrSensor(LDRData &data);
