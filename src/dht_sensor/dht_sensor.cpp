// src/dht_sensor/dht_sensor.cpp

#include "dht_sensor.h"
#include "bit_utils.h"
#include "config.h"
#include <DHT.h>

// ---------------------------------

#define DHT_TYPE DHT22

#define DHT_TEMPERATURE_VALID_MIN -30
#define DHT_TEMPERATURE_VALID_MAX 70
#define DHT_HUMIDITY_VALID_MIN 10
#define DHT_HUMIDITY_VALID_MAX 90

#define DHT_TEMPERATURE_ALARM_MIN -10
#define DHT_TEMPERATURE_ALARM_MAX 40
#define DHT_HUMIDITY_ALARM_MIN 20
#define DHT_HUMIDITY_ALARM_MAX 80

#define STATUS_DHT_OK 0b00000000
#define STATUS_DHT_DEVICE_ERROR 0b00000001
#define STATUS_DHT_DATA_VALID_ERROR 0b00000010
#define STATUS_DHT_TEMPERATURE_ALARM_MIN 0b00010000
#define STATUS_DHT_TEMPERATURE_ALARM_MAX 0b00100000
#define STATUS_DHT_HUMIDITY_ALARM_MIN 0b01000000
#define STATUS_DHT_HUMIDITY_ALARM_MAX 0b10000000

DHT dht(DHT_PIN, DHT_TYPE);

// ---------------------------------

void readDhtData(float &temperature, float &humidity);
uint8_t validateDhtData(const float &temperature, const float &humidity);
void checkDhtTemperatureAlarm(uint8_t &status, const float &temperature);
void checkDhtHumidityAlarm(uint8_t &status, const float &humidity);

// ---------------------------------

void initDhtSensor() { dht.begin(); }

void handleDhtSensor(DHTData &data) {
  float temperature;
  float humidity;
  readDhtData(temperature, humidity);
  uint8_t status = validateDhtData(temperature, humidity);
  data.temperature = temperature;
  data.humidity = humidity;
  data.status = status;
}

void readDhtData(float &temperature, float &humidity) {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}

uint8_t validateDhtData(const float &temperature, const float &humidity) {
  uint8_t status = STATUS_DHT_OK;

  if (isnan(temperature) || isnan(humidity)) {
    setBitsMask(status, STATUS_DHT_DEVICE_ERROR);
    return status;
  }

  if (temperature < DHT_TEMPERATURE_VALID_MIN ||
      temperature > DHT_TEMPERATURE_VALID_MAX ||
      humidity < DHT_HUMIDITY_VALID_MIN || humidity > DHT_HUMIDITY_VALID_MAX) {
    setBitsMask(status, STATUS_DHT_DATA_VALID_ERROR);
  }

  checkDhtTemperatureAlarm(status, temperature);
  checkDhtHumidityAlarm(status, humidity);

  return status;
}

void checkDhtTemperatureAlarm(uint8_t &status, const float &temperature) {
  if (temperature < DHT_TEMPERATURE_ALARM_MIN) {
    setBitsMask(status, STATUS_DHT_TEMPERATURE_ALARM_MIN);
  } else if (temperature > DHT_TEMPERATURE_ALARM_MAX) {
    setBitsMask(status, STATUS_DHT_TEMPERATURE_ALARM_MAX);
  }
}

void checkDhtHumidityAlarm(uint8_t &status, const float &humidity) {
  if (humidity < DHT_HUMIDITY_ALARM_MIN) {
    setBitsMask(status, STATUS_DHT_HUMIDITY_ALARM_MIN);
  } else if (humidity > DHT_HUMIDITY_ALARM_MAX) {
    setBitsMask(status, STATUS_DHT_HUMIDITY_ALARM_MAX);
  }
}