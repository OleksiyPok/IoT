// src/dht_sensor/dht_sensor.cpp

#include <DHT.h>

#include "../hardware_config.h"
#include "../indication/indication.h"
#include "../time/time.h"
#include "config.h"
#include "dht_config.h"
#include "dht_sensor.h"

// ---------------------------------
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

// ---------------------------------
void initDhtSensor(DHTData &data) {
  pinMode(DHT_PIN, INPUT);
  dht.begin();
  data.status |= STATUS_DHT_ERR_INIT;
}

void handleDhtSensor(DHTData &data) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  uint16_t status = data.status & STATUS_DHT_DATA_STALE;

  // Is NaN
  if (isnan(temperature) || isnan(humidity)) {
    status |= STATUS_DHT_DEVICE_ERR;
    status |= STATUS_DHT_DATA_VALID_ERR;
    data.status = status;
    return;
  }

  // Temperature validation
  if (temperature < DHT_TEMPERATURE_VALID_MIN ||
      temperature > DHT_TEMPERATURE_VALID_MAX) {
    status |= STATUS_DHT_DEVICE_ERR;
    status |= STATUS_DHT_DATA_VALID_ERR;
  }

  // Humidity validation
  if (humidity < DHT_HUMIDITY_VALID_MIN || humidity > DHT_HUMIDITY_VALID_MAX) {
    status |= STATUS_DHT_DEVICE_ERR;
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

  status &= ~STATUS_DHT_DATA_STALE;

  data.updated = getCurrentTimestamp();
  data.uptime = millis() / 1000;

  data.temperature = temperature;
  data.humidity = humidity;
  data.status = status;
}
