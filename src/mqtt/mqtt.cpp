// src/mqtt/mqtt.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "../telemetry/telemetry.h"
#include "../telemetry/telemetry_serializer.h"
#include "../wifi/wifi.h"
#include "mqtt.h"

// ---------------------------------

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static uint32_t mqttLastReconnectAt = 0;
static int32_t mqttLastStatus = MQTT_DISCONNECTED;

void printMqttStatus(int32_t mqttStatus);

// ---------------------------------

void initMqtt() {

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
}

// ---------------------------------

bool isMqttConnected() { return mqttClient.connected(); }

// ---------------------------------

void handleMqtt(const Telemetry &telemetryData) {

  if (!isWifiConnected()) {
    return;
  }

  const uint32_t now = millis();
  const int32_t mqttStatus = mqttClient.state();

  if (mqttClient.connected()) {
    if (mqttStatus != mqttLastStatus) {
      Serial.println("[MQTT] Connected");
      mqttLastStatus = mqttStatus;
    }
    mqttClient.loop();
    publishData(telemetryData);
    return;
  }

  if (mqttStatus != mqttLastStatus) {
    Serial.print("[MQTT] Status: ");
    Serial.print(mqttStatus);
    printMqttStatus(mqttStatus);
    mqttLastStatus = mqttStatus;
  }

  if (now - mqttLastReconnectAt < MQTT_RECONNECT_INTERVAL_MS) {
    return;
  }

  connectMQTT();
}

// ---------------------------------

bool connectMQTT() {

  const uint32_t now = millis();

  mqttLastReconnectAt = now;

  Serial.print("[MQTT] Connecting to ");
  Serial.print(MQTT_BROKER);
  Serial.println("...");

  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    return true;
  }

  return false;
}

// ---------------------------------

void publishData(const Telemetry &telemetryData) {

  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return;
  }

  char payload[256];

  if (!serializeTelemetry(telemetryData, payload, sizeof(payload))) {
    Serial.println("[MQTT] Failed to serialize telemetry");
    return;
  }

  Serial.print("[MQTT] Publishing: ");
  Serial.println(payload);

  bool ok = mqttClient.publish(TOPIC_SENSORS, payload);

  Serial.println(ok ? "[MQTT] OK" : "[MQTT] Publish error");
}

// ---------------------------------

void printMqttStatus(int32_t mqttStatus) {

  switch (mqttStatus) {
  case MQTT_CONNECTED:
    Serial.println(" (CONNECTED)");
    break;
  case MQTT_CONNECTION_TIMEOUT:
    Serial.println(" (CONNECTION_TIMEOUT)");
    break;
  case MQTT_CONNECTION_LOST:
    Serial.println(" (CONNECTION_LOST)");
    break;
  case MQTT_CONNECT_FAILED:
    Serial.println(" (CONNECT_FAILED)");
    break;
  case MQTT_DISCONNECTED:
    Serial.println(" (DISCONNECTED)");
    break;
  case MQTT_CONNECT_BAD_PROTOCOL:
    Serial.println(" (BAD_PROTOCOL)");
    break;
  case MQTT_CONNECT_BAD_CLIENT_ID:
    Serial.println(" (BAD_CLIENT_ID)");
    break;
  case MQTT_CONNECT_UNAVAILABLE:
    Serial.println(" (UNAVAILABLE)");
    break;
  case MQTT_CONNECT_BAD_CREDENTIALS:
    Serial.println(" (BAD_CREDENTIALS)");
    break;
  case MQTT_CONNECT_UNAUTHORIZED:
    Serial.println(" (UNAUTHORIZED)");
    break;
  default:
    Serial.println(" (UNKNOWN)");
    break;
  }
}