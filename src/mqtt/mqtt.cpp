// src/mqtt/mqtt.cpp

#include "mqtt.h"

#include <Arduino.h>

#include "../wifi/wifi.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------
void handleMqttSensors(const Telemetry &telemetry) {
  if (!isMqttConnected()) {
    return;
  }

  if (!(telemetry.status & STATUS_DEVICE_SILENT_MODE)) {
    // publishSensors(telemetry);
    publishTelemetry(telemetry);
  } else {
    publishStatus(telemetry);
  }
}

void handleMqttCommands() {
  if (!isMqttConnected()) {
    return;
  }

  publishCommands();
}
