// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>

#include "../commands_out/commands_out.h"
#include "../indication/indication.h"
#include "../monitor/monitor.h"
#include "../serialization/serialization.h"
#include "../telemetry/telemetry.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------

static uint8_t MqttMessageSequence = 0;
static bool publishMqttMessage(const char *topic, const char *payload);

#define TELEMETRY_PAYLOAD_BUFFER_SIZE 512
#define SENSORS_PAYLOAD_BUFFER_SIZE 256
#define STATUS_PAYLOAD_BUFFER_SIZE 256
#define COMMANDS_PAYLOAD_BUFFER_SIZE 128
#define EVENTS_PAYLOAD_BUFFER_SIZE 128

// ---------------------------------

bool publishStatus(const Telemetry &telemetry) {
  char payload[STATUS_PAYLOAD_BUFFER_SIZE];
  const uint8_t sequence = MqttMessageSequence;

  if (!serializeStatus(telemetry, sequence, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize status");
    return false;
  }

  if (!publishMqttMessage(TOPIC_TELEMETRY, payload)) {
    return false;
  }

  MqttMessageSequence++;

  return true;
}

bool publishTelemetry(const Telemetry &telemetry) {
  char payload[TELEMETRY_PAYLOAD_BUFFER_SIZE];
  const uint8_t sequence = MqttMessageSequence;

  if (!serializeTelemetry(telemetry, sequence, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize telemetry");
    return false;
  }

  if (!publishMqttMessage(TOPIC_TELEMETRY, payload)) {
    return false;
  }

  MqttMessageSequence++;

  return true;
}

bool publishCommands() {
  const char *command = getOutgoingCommand();

  if (command == nullptr) {
    return false;
  }

  char payload[COMMANDS_PAYLOAD_BUFFER_SIZE];

  if (!serializeCommands(command, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize command");
    return false;
  }

  if (!publishMqttMessage(TOPIC_COMMANDS_OUT, payload)) {
    return false;
  }

  clearOutgoingCommand();

  return true;
}

bool publishLedChanged(bool isOn) {
  return publishEvent("led_changed", isOn ? "on" : "off");
}

bool publishEvent(const char *event, const char *value) {
  char payload[EVENTS_PAYLOAD_BUFFER_SIZE];

  if (!serializeEvent(event, value, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize event");
    return false;
  }

  return publishMqttMessage(TOPIC_EVENTS, payload);
}

static bool publishMqttMessage(const char *topic, const char *payload) {

  const bool ok = mqttPublish(topic, payload);

  if (ok) {
    requestIndication(IndicationType::MQTT_PUBLISH);
#if defined(DEBUG_MODE)
    Serial.println("[MQTT] Published:");
    Serial.print("       \"");
    Serial.print(topic);
    Serial.println("\"");

    printMonitorPayload(payload);
#endif
  }

  return ok;
}
