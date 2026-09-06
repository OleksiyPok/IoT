// src/mqtt/mqtt.h

#pragma once

#include "../telemetry/telemetry.h"

#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32-demo-a"
#define MQTT_TOP_TOPIC "iot-course/OleksiiPok"
#define TOPIC_TELEMETRY MQTT_TOP_TOPIC "/telemetry"
#define TOPIC_SENSORS_TEMPERATURE MQTT_TOP_TOPIC "/sensors/temperature"
#define TOPIC_SENSORS_HUMIDITY MQTT_TOP_TOPIC "/sensors/humidity"
// #define TOPIC_ACTUATORS_LED MQTT_TOP_TOPIC "/actuators/led"
// #define TOPIC_STATUS MQTT_TOP_TOPIC "/status"
#define TOPIC_STATUS MQTT_TOP_TOPIC "/commands"

#define MQTT_RECONNECT_INTERVAL_MS 5000
#define MQTT_CONNECTION_TIMEOUT_MS 5000
#define MQTT_BUFFER_SIZE 512

void initMqtt();
bool isMqttConnected();
bool connectMQTT();
void handleMqtt(const Telemetry &telemetryData);