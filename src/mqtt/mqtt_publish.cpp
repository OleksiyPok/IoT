// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>

#include "../telemetry/telemetry.h"
#include "../telemetry/telemetry_serializer.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------

static bool publishMqttMessage(const char *topic, const char *payload) {
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(topic);
  Serial.println(payload);

  const bool ok = mqttPublish(topic, payload);

  Serial.println(ok ? "[MQTT] ok" : "[MQTT] Publish error");
  Serial.println();

  return ok;
}

void publishTelemetry(const Telemetry &telemetryData) {
  char payload[MQTT_BUFFER_SIZE];

  if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize telemetry");
    return;
  }

  publishMqttMessage(TOPIC_TELEMETRY, payload);
}

void publishSensorTemperature(const Telemetry &telemetryData) {
  char payload[16];

  snprintf(payload, sizeof(payload), "%.1f", telemetryData.dht.temperature);

  publishMqttMessage(TOPIC_SENSORS_TEMPERATURE, payload);
}

void publishSensorHumidity(const Telemetry &telemetryData) {
  char payload[16];

  snprintf(payload, sizeof(payload), "%.1f", telemetryData.dht.humidity);

  publishMqttMessage(TOPIC_SENSORS_HUMIDITY, payload);
}

void publishSensorLux(const Telemetry &telemetryData) {
  char payload[16];

  snprintf(payload, sizeof(payload), "%.1f", telemetryData.ldr.lux);

  publishMqttMessage(TOPIC_SENSORS_LUX, payload);
}