// src/mqtt/mqtt_aws_service.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#include "../secrets.h"
#include "mqtt_aws_service.h"
#include "mqtt_config.h"

// ---------------------------------

static WiFiClientSecure wifiClientSecure;
static PubSubClient mqttClient(wifiClientSecure);

// ---------------------------------

static MqttMessageCallback mqttMessageCallback = nullptr;

static void handleMqttMessage(char *topic, uint8_t *payload,
                              unsigned int length) {

  if (mqttMessageCallback == nullptr) {
    return;
  }

  mqttMessageCallback(topic, payload, length);
}

// ---------------------------------

void MqttAwsService::init() {

  wifiClientSecure.setCACert(AWS_CERT_CA);
  wifiClientSecure.setCertificate(AWS_CERT_CRT);
  wifiClientSecure.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, MQTT_AWS_PORT);
  mqttClient.setKeepAlive(MQTT_KEEP_ALIVE);
  mqttClient.setSocketTimeout(MQTT_SOCKET_TIMEOUT_SEC);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

// ---------------------------------

bool MqttAwsService::isConnected() { return mqttClient.connected(); }

// ---------------------------------

bool MqttAwsService::connect(const char *clientId) {
  // return mqttClient.connect(AWS_THINGNAME);
  return mqttClient.connect(clientId);
}

// ---------------------------------

void MqttAwsService::disconnect() {
  mqttClient.disconnect();
  wifiClientSecure.stop();
}

// ---------------------------------

void MqttAwsService::loop() { mqttClient.loop(); }

// ---------------------------------

bool MqttAwsService::publish(const char *topic, const char *payload) {

  if (!mqttClient.connected()) {
    return false;
  }

  return mqttClient.publish(topic, payload);
}

// ---------------------------------

bool MqttAwsService::subscribe(const char *topic) {

  if (!mqttClient.connected() || topic == nullptr) {
    return false;
  }

  return mqttClient.subscribe(topic);
}

void MqttAwsService::setCallback(MqttMessageCallback callback) {

  mqttMessageCallback = callback;
  mqttClient.setCallback(handleMqttMessage);
}

// ---------------------------------

MqttStatus MqttAwsService::status() {

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