// src/mqtt/mqtt.cpp

#include "mqtt.h"
#include "../telemetry/telemetry.h"
#include "../wifi/wifi.h"
#include "mqtt_connection.h"
#include "mqtt_publish.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

static int32_t mqttLastStatus = MQTT_DISCONNECTED;

void handleMqtt(const Telemetry &telemetryData) {
  if (!isWifiConnected()) {
    return;
  }

  PubSubClient &mqttClient = getMqttClient();

  const uint32_t now = millis();
  const int32_t mqttStatus = mqttClient.state();

  if (mqttClient.connected()) {
    if (mqttStatus != mqttLastStatus) {
      Serial.println("[MQTT] Connected");
      Serial.println();
      mqttLastStatus = mqttStatus;
    }
    mqttClient.loop();
    publishTelemetry(telemetryData);
    publishSensorTemperature(telemetryData);
    publishSensorHumidity(telemetryData);
    return;
  }

  if (mqttStatus != mqttLastStatus) {
    Serial.print("[MQTT] Status: ");
    Serial.print(mqttStatus);
    printMqttStatus(mqttStatus);
    mqttLastStatus = mqttStatus;
  }

  if (!isMqttReconnectAllowed(now)) {
    return;
  }
  connectMQTT();
}