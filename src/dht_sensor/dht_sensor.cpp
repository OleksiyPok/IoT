// src/dht_sensor/dht_sensor.cpp

#include "dht_sensor.h"
#include "config.h"
#include <DHT.h>

DHT dht(DHT_PIN, DHT_TYPE);
void dhtReadData(DHTData &data);

void initDhtSensor() { dht.begin(); }

void handleDhtSensor(DHTData &data) {
  dhtReadData(data);
  //   validateDhtSensorData(data);
}

void dhtReadData(DHTData &data) {
  data.humidity = dht.readHumidity();
  data.temperature = dht.readTemperature();
}

// void validateDhtSensorData(DHTData &data) {
//   if (isnan(data.temperature) || isnan(data.humidity) ||
//       data.temperature < -40 || data.temperature > 80 || data.humidity < 0 ||
//       data.humidity > 100) {
//     status |= STATUS_DHT_ERR;
//   }
// }