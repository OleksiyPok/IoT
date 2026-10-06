// src/mqtt/mqtt_test_service.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "../secrets.h"
#include "mqtt_config.h"
#include "mqtt_test_service.h"

// ---------------------------------

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

static MqttMessageCallback mqttMessageCallback = nullptr;

static void handleMqttMessage(char *topic, uint8_t *payload,
                              unsigned int length) {

  if (mqttMessageCallback == nullptr) {
    return;
  }

  mqttMessageCallback(topic, payload, length);
}

// ---------------------------------

void MqttTestService::init() {
  mqttClient.setServer(MQTT_TEST_BROKER, MQTT_TEST_PORT);
  mqttClient.setKeepAlive(MQTT_KEEP_ALIVE);
  mqttClient.setSocketTimeout(MQTT_SOCKET_TIMEOUT_SEC);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

bool MqttTestService::isConnected() { return mqttClient.connected(); }

bool MqttTestService::connect(const char *clientId) {
  // return mqttClient.connect(MQTT_TEST_CLIENT_ID);
  return mqttClient.connect(clientId);
}

void MqttTestService::disconnect() { mqttClient.disconnect(); }

void MqttTestService::loop() { mqttClient.loop(); }

bool MqttTestService::publish(const char *topic, const char *payload) {
  if (!mqttClient.connected()) {
    return false;
  }

  return mqttClient.publish(topic, payload);
}

bool MqttTestService::subscribe(const char *topic) {

  if (!mqttClient.connected() || topic == nullptr) {
    return false;
  }

  return mqttClient.subscribe(topic);
}

void MqttTestService::setCallback(MqttMessageCallback callback) {

  mqttMessageCallback = callback;
  mqttClient.setCallback(handleMqttMessage);
}

MqttStatus MqttTestService::status() {
  switch (mqttClient.state()) {
  case MQTT_CONNECTED:
    return MqttStatus::Connected;

  case MQTT_CONNECTION_TIMEOUT:
    return MqttStatus::ConnectionTimeout;

  case MQTT_CONNECTION_LOST:
    return MqttStatus::ConnectionLost;

  case MQTT_CONNECT_FAILED:
    return MqttStatus::ConnectFailed;

  case MQTT_DISCONNECTED:
    return MqttStatus::Disconnected;

  case MQTT_CONNECT_BAD_PROTOCOL:
    return MqttStatus::BadProtocol;

  case MQTT_CONNECT_BAD_CLIENT_ID:
    return MqttStatus::BadClientId;

  case MQTT_CONNECT_UNAVAILABLE:
    return MqttStatus::Unavailable;

  case MQTT_CONNECT_BAD_CREDENTIALS:
    return MqttStatus::BadCredentials;

  case MQTT_CONNECT_UNAUTHORIZED:
    return MqttStatus::Unauthorized;

  default:
    return MqttStatus::Unknown;
  }
}