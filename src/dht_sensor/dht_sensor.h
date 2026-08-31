// src/dht_sensor/dht_sensor.h

#include <Arduino.h>

#pragma once

#include "../telemetry/telemetry.h"
#define STATUS_DHT_OK (0U)

#define STATUS_DHT_DEVICE_ERR (1U << 0)
#define STATUS_DHT_DATA_VALID_ERR (1U << 1)

#define STATUS_DHT_TEMPERATURE_ALARM_MIN (1U << 4)
#define STATUS_DHT_TEMPERATURE_ALARM_MAX (1U << 5)

#define STATUS_DHT_HUMIDITY_ALARM_MIN (1U << 6)
#define STATUS_DHT_HUMIDITY_ALARM_MAX (1U << 7)

#define STATUS_DHT_INIT_ERR                                                    \
  (STATUS_DHT_DEVICE_ERR | STATUS_DHT_DATA_VALID_ERR |                         \
   STATUS_DHT_TEMPERATURE_ALARM_MIN | STATUS_DHT_TEMPERATURE_ALARM_MAX |       \
   STATUS_DHT_HUMIDITY_ALARM_MIN | STATUS_DHT_HUMIDITY_ALARM_MAX)

void initDhtSensor(DHTData &data);
void handleDhtSensor(DHTData &data);
