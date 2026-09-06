// src/mqtt/mqtt_publish.cpp

#include <Arduino.h>
#include <PubSubClient.h>

#include "../telemetry/telemetry.h"
#include "../telemetry/telemetry_serializer.h"
#include "mqtt.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"

// ---------------------------------

void publishTelemetry(const Telemetry &telemetryData) {
  PubSubClient &mqttClient = getMqttClient();
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return;
  }
  char payload[MQTT_BUFFER_SIZE];
  if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize telemetry");
    return;
  }

  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(TOPIC_TELEMETRY);
  Serial.println(payload);
  bool ok = mqttClient.publish(TOPIC_TELEMETRY, payload);
  Serial.println(ok ? "[MQTT] ok" : "[MQTT] Publish error");
  Serial.println();
}

void publishSensorTemperature(const Telemetry &telemetryData) {
  PubSubClient &mqttClient = getMqttClient();
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return;
  }
  char payload[16];
  snprintf(payload, sizeof(payload), "%.1f", telemetryData.dht.temperature);
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(TOPIC_SENSORS_TEMPERATURE);
  Serial.println(payload);
  bool ok = mqttClient.publish(TOPIC_SENSORS_TEMPERATURE, payload);
  Serial.println(ok ? "[MQTT] ok" : "[MQTT] Publish error");
  Serial.println();
}

void publishSensorHumidity(const Telemetry &telemetryData) {
  PubSubClient &mqttClient = getMqttClient();
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return;
  }
  char payload[16];
  snprintf(payload, sizeof(payload), "%.1f", telemetryData.dht.humidity);
  Serial.println("[MQTT] Publishing to the topic:");
  Serial.println(TOPIC_SENSORS_HUMIDITY);
  Serial.println(payload);
  bool ok = mqttClient.publish(TOPIC_SENSORS_HUMIDITY, payload);
  Serial.println(ok ? "[MQTT] ok" : "[MQTT] Publish error");
  Serial.println("------------");
}