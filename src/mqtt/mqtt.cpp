// src/mqtt/mqtt.cpp

#include "mqtt.h"

#include <Arduino.h>

#include "../system/system_state.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------
void handleMqttSensors(const Telemetry &telemetry) {
  if (!isMqttConnected()) {
    return;
  }

  if (!(telemetry.systemState & SYSTEM_SILENT_MASK)) {
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

bool publishManualTrigger(const Telemetry &telemetry) {
  const bool telemetryPublished = publishTelemetry(telemetry);
  const bool eventPublished = publishEvent("manual_trigger");

  return telemetryPublished && eventPublished;
}