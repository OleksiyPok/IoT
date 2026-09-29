// src/mqtt/mqtt_connection.cpp

#include <Arduino.h>

#include "../system/system_state.h"
#include "../time/time.h"
#include "../wifi/wifi.h"
#include "mqtt_config.h"
#include "mqtt_connection.h"

// ---------------------------------

static MqttServiceType mqttService;

static MqttStatus mqttLastStatus = MqttStatus::Disconnected;
static uint32_t mqttLastConnectAttemptAt = 0;
static uint32_t mqttNextConnectionCycleAt = 0;
static uint8_t mqttConnectionAttempts = 0;
static bool mqttInitialized = false;

static bool connectMQTT(uint64_t deviceId);
static void handleMqttStatus();
static void printMqttStatus(MqttStatus mqttStatus);

// ---------------------------------

void initMqtt() {

  if (mqttInitialized) {
    return;
  }

  mqttService.init();
  mqttInitialized = true;
}

bool isMqttConnected() {
  return isWifiConnected() && mqttInitialized && mqttService.isConnected();
}

static bool connectMQTT(uint64_t deviceId) {
  const uint32_t now = millis();

  mqttLastConnectAttemptAt = now;
  mqttConnectionAttempts++;

  Serial.print("[MQTT] Connecting");
  Serial.print(" (attempt ");
  Serial.print(mqttConnectionAttempts);
  Serial.print("/");
  Serial.print(MQTT_MAX_CONNECTION_ATTEMPTS);
  Serial.println(")...");

  char clientId[21];
  snprintf(clientId, sizeof(clientId), "%llu",
           static_cast<unsigned long long>(deviceId));

  const bool connected = mqttService.connect(clientId);

  if (connected) {
    mqttLastStatus = MqttStatus::Connected;

    Serial.println("[MQTT] Connected");
    Serial.println();

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;

    return true;
  }

  handleMqttStatus();

  return false;
}

bool handleMqttConnection(const Telemetry &telemetry) {

  const uint32_t now = millis();

  // Wi-Fi and time synchronization are required for MQTT.
  if (!isWifiConnected() || !isTimeSynchronized()) {

    if (mqttInitialized && mqttService.isConnected()) {
      mqttService.disconnect();
    }

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;

    if (mqttLastStatus != MqttStatus::Disconnected) {
      Serial.println(
          "[MQTT] Status: (DISCONNECTED - prerequisite unavailable)");
      mqttLastStatus = MqttStatus::Disconnected;
    }

    return false;
  }

  if (!mqttInitialized) {
    return false;
  }

  handleMqttStatus();

  if (mqttService.isConnected()) {

    mqttConnectionAttempts = 0;
    mqttNextConnectionCycleAt = 0;

    mqttService.loop();

    return true;
  }

  handleMqttStatus();

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
    Serial.print(" attempts failed - waiting ");
    Serial.print(MQTT_RECONNECT_CYCLE_DELAY_MS / 1000 / 60);
    Serial.println(" minutes");

    return false;
  }

  if (now - mqttLastConnectAttemptAt < MQTT_RECONNECT_INTERVAL_MS) {
    return false;
  }

  return connectMQTT(telemetry.device_id);
}

static void handleMqttStatus() {

  const MqttStatus mqttStatus = mqttService.status();

  if (mqttStatus == MqttStatus::Connected) {
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
