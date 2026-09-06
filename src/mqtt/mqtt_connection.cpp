// src/mqtt/mqtt_connection.cpp

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "mqtt.h"
#include "mqtt_connection.h"

// ---------------------------------

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static uint32_t mqttLastReconnectAt = 0;

// ---------------------------------

PubSubClient &getMqttClient() { return mqttClient; }

void initMqtt() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(30);
  mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

bool isMqttConnected() { return mqttClient.connected(); }

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

bool isMqttReconnectAllowed(uint32_t now) {
  return now - mqttLastReconnectAt >= MQTT_RECONNECT_INTERVAL_MS;
}

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