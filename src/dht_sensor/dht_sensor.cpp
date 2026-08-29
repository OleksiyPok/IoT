// src/dht_sensor/dht_sensor.cpp

#include "dht_sensor.h"
#include "../indication/indication.h"
#include "config.h"
#include <DHT.h>

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

void initDhtSensor() { dht.begin(); }

void handleDhtSensor(DHTData &data, uint8_t &ledState) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  uint8_t status = 0;

  // Is NaN
  if (isnan(temperature) || isnan(humidity)) {

    ledState &= ~(LED_TEMPERATURE_MIN | LED_TEMPERATURE_MAX);
    status |= STATUS_DHT_DEVICE_ERROR;

    data.temperature = -1;
    data.humidity = -1;
    data.status = status;

    return;
  }

  // Temperature and humidity validation
  if (temperature < DHT_TEMPERATURE_VALID_MIN ||
      temperature > DHT_TEMPERATURE_VALID_MAX ||
      humidity < DHT_HUMIDITY_VALID_MIN || humidity > DHT_HUMIDITY_VALID_MAX) {
    status |= STATUS_DHT_DATA_VALID_ERROR;
  }

  // Temperature alarm
  if (temperature < DHT_TEMPERATURE_ALARM_MIN) {
    ledState &= ~LED_TEMPERATURE_MAX;
    ledState |= LED_TEMPERATURE_MIN;
    status |= STATUS_DHT_TEMPERATURE_ALARM_MIN;
  } else if (temperature > DHT_TEMPERATURE_ALARM_MAX) {
    ledState &= ~LED_TEMPERATURE_MIN;
    ledState |= LED_TEMPERATURE_MAX;
    status |= STATUS_DHT_TEMPERATURE_ALARM_MAX;
  } else {
    ledState &= ~(LED_TEMPERATURE_MIN | LED_TEMPERATURE_MAX);
  }

  // Humidity alarm
  if (humidity < DHT_HUMIDITY_ALARM_MIN) {
    ledState &= ~LED_HUMIDITY_MAX;
    ledState |= LED_HUMIDITY_MIN;
    status |= STATUS_DHT_HUMIDITY_ALARM_MIN;
  } else if (humidity > DHT_HUMIDITY_ALARM_MAX) {
    ledState &= ~LED_HUMIDITY_MIN;
    ledState |= LED_HUMIDITY_MAX;
    status |= STATUS_DHT_HUMIDITY_ALARM_MAX;
  } else {
    ledState &= ~(LED_HUMIDITY_MIN | LED_HUMIDITY_MAX);
  }

  data.temperature = temperature;
  data.humidity = humidity;
  data.status = status;
}
