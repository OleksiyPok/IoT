// src/dht_sensor/dht_sensor.cpp

#include <DHT.h>

#include "../indication/indication.h"
#include "config.h"
#include "dht_sensor.h"

// ---------------------------------

#define DHT_TYPE DHT22

#define DHT_TEMPERATURE_VALID_MIN -30
#define DHT_TEMPERATURE_ALARM_MIN -10
#define DHT_TEMPERATURE_ALARM_MAX 40
#define DHT_TEMPERATURE_VALID_MAX 70

#define DHT_HUMIDITY_VALID_MIN 10
#define DHT_HUMIDITY_ALARM_MIN 20
#define DHT_HUMIDITY_ALARM_MAX 80
#define DHT_HUMIDITY_VALID_MAX 90

DHT dht(DHT_PIN, DHT_TYPE);

// ---------------------------------

void initDhtSensor(DHTData &data) {
  pinMode(DHT_PIN, INPUT);
  dht.begin();
  data.status |= STATUS_DHT_INIT_ERR;
}

void handleDhtSensor(DHTData &data) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  uint8_t status = STATUS_DHT_OK;

  // Is NaN
  if (isnan(temperature) || isnan(humidity)) {
    status |= STATUS_DHT_DEVICE_ERR;
    data.status = status;
    return;
  }

  data.updated = millis();

  // Temperature validation
  if (temperature < DHT_TEMPERATURE_VALID_MIN ||
      temperature > DHT_TEMPERATURE_VALID_MAX) {
    status |= STATUS_DHT_DATA_VALID_ERR;
  }

  // Humidity validation
  if (humidity < DHT_HUMIDITY_VALID_MIN || humidity > DHT_HUMIDITY_VALID_MAX) {
    status |= STATUS_DHT_DATA_VALID_ERR;
  }

  // Temperature alarm
  if (temperature < DHT_TEMPERATURE_ALARM_MIN) {
    status |= STATUS_DHT_TEMPERATURE_ALARM_MIN;
  } else if (temperature > DHT_TEMPERATURE_ALARM_MAX) {
    status |= STATUS_DHT_TEMPERATURE_ALARM_MAX;
  }

  // Humidity alarm
  if (humidity < DHT_HUMIDITY_ALARM_MIN) {
    status |= STATUS_DHT_HUMIDITY_ALARM_MIN;
  } else if (humidity > DHT_HUMIDITY_ALARM_MAX) {
    status |= STATUS_DHT_HUMIDITY_ALARM_MAX;
  }

  data.temperature = temperature;
  data.humidity = humidity;
  data.status = status;
}
