// src/dht_sensor/dht_sensor.h

#pragma once

struct DHTData {
  float temperature; // °C
  float humidity;    // %
};

void initDhtSensor();
void handleDhtSensor(DHTData &data);
