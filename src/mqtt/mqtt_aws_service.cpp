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

void MqttAwsService::init() {

  wifiClientSecure.setCACert(AWS_CERT_CA);
  wifiClientSecure.setCertificate(AWS_CERT_CRT);
  wifiClientSecure.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, MQTT_AWS_PORT);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

// ---------------------------------

bool MqttAwsService::isConnected() { return mqttClient.connected(); }

// ---------------------------------

bool MqttAwsService::connect() { return mqttClient.connect(THINGNAME); }

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