// src/mqtt/mqtt_subscribe.cpp

#include <Arduino.h>

#include "../commands/commands.h"
#include "../serialization/deserializers/commands_deserializer.h"
#include "../serialization/deserializers/sensors_deserializer.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_subscribe.h"

// ---------------------------------
static MqttSensorData mqttSensorData = {0.0f, 0.0f, false, false};

static bool mqttTopicsSubscribed = false;

static void handleCommandsMessage(const char *payload, size_t length);
static void handleSensorsMessage(const char *payload, size_t length);

// ---------------------------------
void initMqttSubscribe() { mqttTopicsSubscribed = false; }

bool subscribeMqttTopics() {
  if (!isMqttConnected()) {
    mqttTopicsSubscribed = false;
    return false;
  }

  if (mqttTopicsSubscribed) {
    return true;
  }

  const bool commandsOk = mqttSubscribe(TOPIC_COMMANDS);
  const bool sensorsOk = mqttSubscribe(TOPIC_SENSORS);

  if (commandsOk && sensorsOk) {
    mqttTopicsSubscribed = true;
    Serial.println("[MQTT] Subscribed to commands and sensors");
    return true;
  }

  Serial.println("[MQTT] Failed to subscribe to one or more topics");
  return false;
}

void handleMqttMessage(char *topic, uint8_t *payload, unsigned int length) {
  if (topic == nullptr || payload == nullptr || length == 0) {
    return;
  }

  // PubSubClient payload is not guaranteed to be null-terminated.
  char message[MQTT_BUFFER_SIZE];
  const size_t copyLength =
      length < sizeof(message) - 1 ? length : sizeof(message) - 1;

  memcpy(message, payload, copyLength);
  message[copyLength] = '\0';

  if (strcmp(topic, TOPIC_COMMANDS) == 0) {
    handleCommandsMessage(message, copyLength);
    return;
  }

  if (strcmp(topic, TOPIC_SENSORS) == 0) {
    handleSensorsMessage(message, copyLength);
  }
}

static void handleCommandsMessage(const char *payload, size_t length) {
  char command[32];

  if (!deserializeCommand(payload, length, command, sizeof(command))) {
    Serial.println("[MQTT] Failed to deserialize command");
    return;
  }

  setReceivedCommand(command);
}

static void handleSensorsMessage(const char *payload, size_t length) {
  MqttSensorData incoming = {0.0f, 0.0f, false, false};

  if (!deserializeSensors(payload, length, incoming)) {
    Serial.println("[MQTT] Failed to deserialize sensor data");
    return;
  }

  if (incoming.hasTemperature) {
    mqttSensorData.temperature = incoming.temperature;
    mqttSensorData.hasTemperature = true;
  }

  if (incoming.hasLdrLux) {
    mqttSensorData.ldrLux = incoming.ldrLux;
    mqttSensorData.hasLdrLux = true;
  }
}

const MqttSensorData &getMqttSensorData() { return mqttSensorData; }