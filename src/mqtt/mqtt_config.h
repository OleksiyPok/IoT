// src/mqtt/mqtt_config.h

#pragma once

// ---------------------------------
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "OleksiiPok-esp32-b"

#define MQTT_TOP_TOPIC "iot-course/OleksiiPok"

#define TOPIC_COMMANDS MQTT_TOP_TOPIC "/commands"
#define TOPIC_SENSORS MQTT_TOP_TOPIC "/sensors"

#define MQTT_RECONNECT_INTERVAL_MS 5000
#define MQTT_MAX_CONNECTION_ATTEMPTS 3
#define MQTT_RECONNECT_CYCLE_DELAY_MS 300000

#define MQTT_BUFFER_SIZE 512
