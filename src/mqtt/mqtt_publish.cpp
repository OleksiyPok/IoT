// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>

#include "../monitor/monitor.h"
#include "../serialization/serialization.h"
#include "../telemetry/telemetry.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------

static bool publishMqttMessage(const char *topic, const char *payload) {
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(topic);
  printMonitorPayload(payload);

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
  if (!serializeTemperature(telemetryData.dht.temperature, payload,
                            sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize temperature");
    return;
  }

  publishMqttMessage(TOPIC_SENSORS_TEMPERATURE, payload);
}

void publishSensorHumidity(const Telemetry &telemetryData) {
  char payload[16];
  if (!serializeHumidity(telemetryData.dht.humidity, payload,
                         sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize humidity");
    return;
  }

  publishMqttMessage(TOPIC_SENSORS_HUMIDITY, payload);
}

void publishSensorLux(const Telemetry &telemetryData) {
  char payload[16];
  if (!serializeLux(telemetryData.ldr.lux, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize lux");
    return;
  }
  publishMqttMessage(TOPIC_SENSORS_LUX, payload);
}