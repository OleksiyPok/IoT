// src/mqtt/mqtt_connection.cpp

#include <Arduino.h>

#include "../system/system_state.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"

// ---------------------------------

static MqttServiceType mqttService;

static MqttStatus mqttLastStatus = MqttStatus::Disconnected;
static uint32_t mqttLastConnectAttemptAt = 0;
static uint32_t mqttNextConnectionCycleAt = 0;
static uint8_t mqttConnectionAttempts = 0;

static bool connectMQTT();
static void handleMqttStatus();
static void printMqttStatus(MqttStatus mqttStatus);

// ---------------------------------

void initMqtt() { mqttService.init(); }

bool isMqttConnected() {
  return isWifiConnected() && mqttService.isConnected(); // ?
}

static bool connectMQTT() {
  const uint32_t now = millis();

  mqttLastConnectAttemptAt = now;
  mqttConnectionAttempts++;

  Serial.print("[MQTT] Connecting");
  Serial.print(" (attempt ");
  Serial.print(mqttConnectionAttempts);
  Serial.print("/");
  Serial.print(MQTT_MAX_CONNECTION_ATTEMPTS);
  Serial.println(")...");

  const bool connected = mqttService.connect();

  return connected;
}

bool handleMqttConnection(Telemetry &telemetry) {

  const uint32_t now = millis();

  if (!isWifiConnected()) {
    if (mqttService.isConnected()) {
      mqttService.disconnect();
    }

    if (mqttLastStatus != MqttStatus::Disconnected) {
      Serial.println("[MQTT] Status:  (DISCONNECTED)");
      mqttLastStatus = MqttStatus::Disconnected;
    }

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;

    return false;
  }

  handleMqttStatus();

  if (mqttService.isConnected()) {

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;

    mqttService.loop();

    return true;
  }

  if (mqttNextConnectionCycleAt != 0) {
    if (now - mqttNextConnectionCycleAt < MQTT_RECONNECT_CYCLE_DELAY_MS) {
      return false;
    }

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;
  }

  if (mqttConnectionAttempts >= MQTT_MAX_CONNECTION_ATTEMPTS) {
    mqttNextConnectionCycleAt = now;

    Serial.print("[MQTT] ");
    Serial.print(MQTT_MAX_CONNECTION_ATTEMPTS);
    Serial.println(" attempts failed - waiting 5 minutes");

    return false;
  }

  if (now - mqttLastConnectAttemptAt < MQTT_RECONNECT_INTERVAL_MS) {
    return false;
  }

  connectMQTT();

  return false;
}

static void handleMqttStatus() {

  const MqttStatus mqttStatus = mqttService.status();

  if (mqttStatus == MqttStatus::Connected) {
    if (mqttStatus != mqttLastStatus) {
      Serial.println("[MQTT] Connected");
      Serial.println();

      mqttLastStatus = mqttStatus;
    }

    return;
  }

  if (mqttStatus != mqttLastStatus) {
    Serial.print("[MQTT] Status: ");
    printMqttStatus(mqttStatus);

    mqttLastStatus = mqttStatus;
  }
}

bool mqttPublish(const char *topic, const char *payload) {
  if (!isMqttConnected()) {
    Serial.println("[MQTT] Not connected - skip publish");
    return false;
  }

  return mqttService.publish(topic, payload);
}

static void printMqttStatus(MqttStatus mqttStatus) {
  switch (mqttStatus) {
  case MqttStatus::Connected:
    Serial.println(" (CONNECTED)");
    break;

  case MqttStatus::ConnectionTimeout:
    Serial.println(" (CONNECTION_TIMEOUT)");
    break;

  case MqttStatus::ConnectionLost:
    Serial.println(" (CONNECTION_LOST)");
    break;

  case MqttStatus::ConnectFailed:
    Serial.println(" (CONNECT_FAILED)");
    break;

  case MqttStatus::Disconnected:
    Serial.println(" (DISCONNECTED)");
    break;

  case MqttStatus::BadProtocol:
    Serial.println(" (BAD_PROTOCOL)");
    break;

  case MqttStatus::BadClientId:
    Serial.println(" (BAD_CLIENT_ID)");
    break;

  case MqttStatus::Unavailable:
    Serial.println(" (UNAVAILABLE)");
    break;

  case MqttStatus::BadCredentials:
    Serial.println(" (BAD_CREDENTIALS)");
    break;

  case MqttStatus::Unauthorized:
    Serial.println(" (UNAUTHORIZED)");
    break;

  case MqttStatus::Unknown:
    Serial.println(" (UNKNOWN)");
    break;
  }
}
