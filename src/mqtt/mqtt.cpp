// src/mqtt/mqtt.cpp

#include "mqtt.h"

#include <Arduino.h>

#include "../wifi/wifi.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------
void handleMqttSensors(const Telemetry &telemetryData) {
  if (!isMqttConnected()) {
    return;
  }

  if (!(telemetryData.status & STATUS_DEVICE_SILENT_MODE)) {
    // publishSensors(telemetryData);
    // publishTelemetry();
  }
  publishStatus(telemetryData);
}

void handleMqttCommands() {
  if (!isMqttConnected()) {
    return;
  }

  publishCommands();
}
