// src/dht_sensor/dht_sensor.h

#include <Arduino.h>

#pragma once

struct DHTData {
  float temperature; // °C
  float humidity;    // %
  uint8_t status;
};

void initDhtSensor();
void handleDhtSensor(DHTData &data);
