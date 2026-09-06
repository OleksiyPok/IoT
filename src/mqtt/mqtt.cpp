// src/mqtt/mqtt.cpp

#include "mqtt.h"

#include <Arduino.h>

#include "../wifi/wifi.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

void handleMqtt(const Telemetry &telemetryData) {
  if (!isWifiConnected()) {
    return;
  }

  if (!handleMqttConnection()) {
    return;
  }

  publishTelemetry(telemetryData);
  publishSensorTemperature(telemetryData);
  publishSensorHumidity(telemetryData);
}