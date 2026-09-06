// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32-demo-a"
#define TOPIC_SENSORS "iot-course/OleksiiPok/sensors"

#define MQTT_RECONNECT_INTERVAL_MS 5000
#define MQTT_CONNECTION_TIMEOUT_MS 5000

void initMqtt();
bool isMqttConnected();
bool connectMQTT();
void publishData(const Telemetry &telemetryData);
void handleMqtt(const Telemetry &telemetryData);