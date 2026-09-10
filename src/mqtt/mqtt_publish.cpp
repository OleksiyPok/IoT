// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>

#include "../monitor/monitor.h"
#include "../serialization/serializers/sensors_serializer.h"
#include "../serialization/serializers/status_serializer.h"
#include "../telemetry/telemetry.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------

static bool publishMqttMessage(const char *topic, const char *payload);

// ---------------------------------
// void publishTelemetry(const Telemetry &telemetryData) {
//   char payload[MQTT_BUFFER_SIZE];

//   if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {
//     Serial.println("[MQTT] Failed to serialize telemetry");
//     return;
//   }

//   publishMqttMessage(TOPIC_TELEMETRY, payload);
// }

void publishSensors(const Telemetry &telemetry) {
  char payload[256];
  if (!serializeSensors(telemetry, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize sensors");
    return;
  }
  publishMqttMessage(TOPIC_SENSORS, payload);
}

void publishStatus(const Telemetry &telemetry) {
  char payload[256];
  if (!serializeStatus(telemetry, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize status");
    return;
  }
  publishMqttMessage(TOPIC_STATUS, payload);
}

static bool publishMqttMessage(const char *topic, const char *payload) {
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(topic);
  printMonitorPayload(payload);

  const bool ok = mqttPublish(topic, payload);

  // Serial.println(ok ? "[MQTT] ok" : "[MQTT] Publish error");
  Serial.println();

  return ok;
}
