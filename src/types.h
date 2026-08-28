// src/types.h

#pragma once

#include "dht_sensor/dht_sensor.h"
#include "ldr_sensor/ldr_sensor.h"

#define STATUS_OK 0b00000000
#define STATUS_LDR_ERR 0b00000001  // bit 0: LDR error
#define STATUS_DHT_ERR 0b00000010  // bit 1: DHT22 error
#define STATUS_WIFI_ERR 0b00000100 // bit 2: Wi-Fi error
#define STATUS_MQTT_ERR 0b00001000 // bit 3: MQTT error

struct Telemetry {
  DHTData dht;
  LDRData ldr;
  unsigned long worktime;
  unsigned char status; // system status register
};