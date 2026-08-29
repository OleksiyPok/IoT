// src/dht_sensor/dht_sensor.h

#pragma once

#include <Arduino.h>

struct DHTData {
  float temperature; // °C
  float humidity;    // %
  uint8_t status;
};

void initDhtSensor();
void handleDhtSensor(DHTData &data, uint8_t &ledState);
