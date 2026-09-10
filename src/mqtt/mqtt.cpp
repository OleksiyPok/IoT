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

  // publishTelemetry(telemetryData);

  if (!(telemetryData.status & STATUS_DEVICE_SILENT_MODE)) {
    publishSensors(telemetryData);
  }

  publishStatus(telemetryData);

  // Serial.println("------------");
}