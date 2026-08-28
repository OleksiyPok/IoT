// src/dht_sensor/dht_sensor.h

#pragma once

#define DHT_TYPE DHT22

struct DHTData {
  float temperature; // °C
  float humidity;    // %
};

void initDhtSensor();
void handleDhtSensor(DHTData &data);
