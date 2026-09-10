// src/mqtt/mqtt_config.h

#pragma once

#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32-demo-a"

#define MQTT_TOP_TOPIC "iot-course/OleksiiPok"
#define TOPIC_TELEMETRY MQTT_TOP_TOPIC "/telemetry"
#define TOPIC_SENSORS_TEMPERATURE MQTT_TOP_TOPIC "/sensors/temperature"
#define TOPIC_SENSORS_HUMIDITY MQTT_TOP_TOPIC "/sensors/humidity"
#define TOPIC_SENSORS_LUX MQTT_TOP_TOPIC "/sensors/lux"
// #define TOPIC_ACTUATORS_LED MQTT_TOP_TOPIC "/actuators/led"
// #define TOPIC_STATUS MQTT_TOP_TOPIC "/commands"
#define TOPIC_SENSORS MQTT_TOP_TOPIC "/sensors"
#define TOPIC_STATUS MQTT_TOP_TOPIC "/status"

#define MQTT_RECONNECT_INTERVAL_MS 5000
#define MQTT_CONNECTION_TIMEOUT_MS 5000

#define MQTT_BUFFER_SIZE 512