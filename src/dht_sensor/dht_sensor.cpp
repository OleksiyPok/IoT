// src/dht_sensor/dht_sensor.cpp

#include "dht_sensor.h"
#include "bit_utils.h"
#include "config.h"
#include <DHT.h>

// ---------------------------------

#define DHT_TYPE DHT22

#define DHT_TEMPERRATURE_VALID_MIN -30
#define DHT_TEMPERRATURE_VALID_MAX 70
#define DHT_HUMIDITY_VALID_MIN 0
#define DHT_HUMIDITY_VALID_MAX 100

#define DHT_TEMPERRATURE_ALARM_MIN -10
#define DHT_TEMPERRATURE_ALARM_MAX 40
#define DHT_HUMIDITY_ALARM_MIN 20
#define DHT_HUMIDITY_ALARM_MAX 80

#define STATUS_DHT_OK 0b00000000
#define STATUS_DHT_DEVICE_ERROR 0b00000001
#define STATUS_DHT_DATA_VALID_ERROR 0b00000010

#define STATUS_DHT_TEMPERRATURE_ALARM_MIN 0b00010000
#define STATUS_DHT_TEMPERRATURE_ALARM_MAX 0b00100000
#define STATUS_DHT_HUMIDITY_ALARM_MIN 0b01000000
#define STATUS_DHT_HUMIDITY_ALARM_MAX 0b10000000

DHT dht(DHT_PIN, DHT_TYPE);

// ---------------------------------

void initDhtSensor() { dht.begin(); }

void handleDhtSensor(DHTData &data) {

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  uint8_t status = STATUS_DHT_OK;

  if (isnan(temperature) || isnan(humidity)) {
    setBitsMask(status, STATUS_DHT_DEVICE_ERROR);
  } else if (temperature < DHT_TEMPERRATURE_VALID_MIN ||
             temperature > DHT_TEMPERRATURE_VALID_MAX ||
             humidity < DHT_HUMIDITY_VALID_MIN ||
             humidity > DHT_HUMIDITY_VALID_MAX) {
    setBitsMask(status, STATUS_DHT_DATA_VALID_ERROR);
  }

  if (temperature < DHT_TEMPERRATURE_ALARM_MIN) {
    setBitsMask(status, STATUS_DHT_TEMPERRATURE_ALARM_MIN);
  } else if (temperature > DHT_TEMPERRATURE_ALARM_MAX) {
    setBitsMask(status, STATUS_DHT_TEMPERRATURE_ALARM_MAX);
  }

  if (humidity < DHT_HUMIDITY_ALARM_MIN) {
    setBitsMask(status, STATUS_DHT_HUMIDITY_ALARM_MIN);
  } else if (humidity > DHT_HUMIDITY_ALARM_MAX) {
    setBitsMask(status, STATUS_DHT_HUMIDITY_ALARM_MAX);
  }

  data.humidity = humidity;
  data.temperature = temperature;
  data.status = status;
}
