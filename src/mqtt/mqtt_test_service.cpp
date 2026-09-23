// src/mqtt/mqtt_test_service.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "../secrets.h"
#include "mqtt_config.h"
#include "mqtt_test_service.h"

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);

void MqttTestService::init() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

bool MqttTestService::isConnected() { return mqttClient.connected(); }

bool MqttTestService::connect() { return mqttClient.connect(MQTT_CLIENT_ID); }

void MqttTestService::disconnect() { mqttClient.disconnect(); }

void MqttTestService::loop() { mqttClient.loop(); }

bool MqttTestService::publish(const char *topic, const char *payload) {
  if (!mqttClient.connected()) {
    return false;
  }

  return mqttClient.publish(topic, payload);
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