// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>

#include "../indication/indication.h"
#include "../monitor/monitor.h"
#include "../serialization/serialization.h"
#include "../telemetry/telemetry.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------
static bool publishMqttMessage(const char *topic, const char *payload);

#define TELEMETRY_PAYLOAD_BUFFER_SIZE 512
#define SENSORS_PAYLOAD_BUFFER_SIZE 256
#define STATUS_PAYLOAD_BUFFER_SIZE 256
#define COMMANDS_PAYLOAD_BUFFER_SIZE 128
// ---------------------------------
bool publishSensors(const Telemetry &telemetry) {
  char payload[SENSORS_PAYLOAD_BUFFER_SIZE];
  if (!serializeSensors(telemetry, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize sensors");
    return true;
  }

  if (!publishMqttMessage(TOPIC_SENSORS, payload)) {
    return false;
  }

  return true;
}

bool publishStatus(const Telemetry &telemetry) {
  char payload[STATUS_PAYLOAD_BUFFER_SIZE];
  if (!serializeStatus(telemetry, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize status");
    return true;
  }

  if (!publishMqttMessage(TOPIC_STATUS, payload)) {
    return false;
  }

  return true;
}

bool publishTelemetry(const Telemetry &telemetry) {
  char payload[TELEMETRY_PAYLOAD_BUFFER_SIZE];
  if (!serializeTelemetry(telemetry, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize sensors");
    return true;
  }

  if (!publishMqttMessage(TOPIC_TELEMETRY, payload)) {
    return false;
  }

  return true;
}

bool publishCommands() {
  const char *command = getCommand();

  if (command == nullptr) {
    return false;
  }

  char payload[COMMANDS_PAYLOAD_BUFFER_SIZE];
  if (!serializeCommands(command, payload, sizeof(payload))) {
    return false;
  }

  if (!publishMqttMessage(TOPIC_COMMANDS, payload)) {
    return false;
  }

  clearCommand();

  return true;
}

static bool publishMqttMessage(const char *topic, const char *payload) {
#if defined(DEBUG_MODE)
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(topic);
  printMonitorPayload(payload);
#endif

  const bool ok = mqttPublish(topic, payload);

  if (ok) {
    requestIndication(IndicationType::MQTT_PUBLISH);
  }

  return ok;
}
