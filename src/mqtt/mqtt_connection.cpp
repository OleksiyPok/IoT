// src/mqtt/mqtt_connection.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt_config.h"
#include "mqtt_connection.h"

// ---------------------------------

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static int32_t mqttLastStatus = MQTT_DISCONNECTED;
static uint32_t mqttLastConnectAttemptAt = 0;
static uint32_t mqttNextConnectionCycleAt = 0;
static uint8_t mqttConnectionAttempts = 0;

static bool connectMQTT();
static void handleMqttStatus();
static void printMqttStatus(int32_t mqttStatus);
// ---------------------------------

void initMqtt() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

bool isMqttConnected() { return mqttClient.connected(); }

static bool connectMQTT() {
  const uint32_t now = millis();
  mqttLastConnectAttemptAt = now;
  mqttConnectionAttempts++;

  Serial.print("[MQTT] Connecting to ");
  Serial.print(MQTT_BROKER);
  Serial.print(" (attempt ");
  Serial.print(mqttConnectionAttempts);
  Serial.print("/");
  Serial.print(MQTT_MAX_CONNECTION_ATTEMPTS);
  Serial.println(")...");

  return mqttClient.connect(MQTT_CLIENT_ID);
}

bool handleMqttConnection() {

  const uint32_t now = millis();

  handleMqttStatus();

  if (mqttClient.connected()) {
    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;
    mqttClient.loop();
    return true;
  }

  // Wait before starting the next cycle
  if (mqttNextConnectionCycleAt != 0) {
    if (now - mqttNextConnectionCycleAt < MQTT_RECONNECT_CYCLE_DELAY_MS) {
      return false;
    }

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;
  }

  // Maximum attempts reached
  if (mqttConnectionAttempts >= MQTT_MAX_CONNECTION_ATTEMPTS) {
    mqttNextConnectionCycleAt = now;
    Serial.print("[MQTT] ");
    Serial.print(MQTT_MAX_CONNECTION_ATTEMPTS);
    Serial.println(" attempts failed - waiting 5 minutes");
    return false;
  }

  // Wait between individual attempts
  if (now - mqttLastConnectAttemptAt < MQTT_RECONNECT_INTERVAL_MS) {
    return false;
  }
  connectMQTT();
  return false;
}

static void handleMqttStatus() {
  const int32_t mqttStatus = mqttClient.state();

  if (mqttStatus == MQTT_CONNECTED) {
    if (mqttStatus != mqttLastStatus) {
      Serial.println("[MQTT] Connected");
      Serial.println();
      mqttLastStatus = mqttStatus;
    }
    return;
  }

  if (mqttStatus != mqttLastStatus) {
    Serial.print("[MQTT] Status: ");
    Serial.print(mqttStatus);
    printMqttStatus(mqttStatus);
    mqttLastStatus = mqttStatus;
  }
}

bool mqttPublish(const char *topic, const char *payload) {
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return false;
  }

  return mqttClient.publish(topic, payload);
}

static void printMqttStatus(int32_t mqttStatus) {
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
