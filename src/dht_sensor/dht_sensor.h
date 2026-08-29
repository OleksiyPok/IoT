// src/dht_sensor/dht_sensor.h

#pragma once

#include "../telemetry/telemetry.h"
#include <Arduino.h>

#define STATUS_DHT_OK 0b00000000

#define STATUS_DHT_DEVICE_ERR 0b00000001
#define STATUS_DHT_DATA_VALID_ERR 0b00000010

#define STATUS_DHT_TEMPERATURE_ALARM_MIN 0b00010000
#define STATUS_DHT_TEMPERATURE_ALARM_MAX 0b00100000

#define STATUS_DHT_HUMIDITY_ALARM_MIN 0b01000000
#define STATUS_DHT_HUMIDITY_ALARM_MAX 0b10000000

#define STATUS_DHT_INIT_ERR                                                    \
  (STATUS_DHT_DEVICE_ERR | STATUS_DHT_DATA_VALID_ERR |                         \
   STATUS_DHT_TEMPERATURE_ALARM_MIN | STATUS_DHT_TEMPERATURE_ALARM_MAX |       \
   STATUS_DHT_HUMIDITY_ALARM_MIN | STATUS_DHT_HUMIDITY_ALARM_MAX)

void initDhtSensor(DHTData &data);
void handleDhtSensor(DHTData &data, uint8_t &ledState);
