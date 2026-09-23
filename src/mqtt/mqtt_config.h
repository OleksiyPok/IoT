// src/mqtt/mqtt_config.h

#pragma once

// Uncomment exactly one service
// #define MQTT_SERVICE_AWS
#define MQTT_SERVICE_TEST

#if defined(MQTT_SERVICE_AWS) && defined(MQTT_SERVICE_TEST)
#error "Only one MQTT service can be selected"
#endif

#if defined(MQTT_SERVICE_TEST)
#include "mqtt_test_service.h"
using MqttServiceType = MqttTestService;
#elif defined(MQTT_SERVICE_AWS)
#include "mqtt_aws_service.h"
using MqttServiceType = MqttAwsService;
#else
#error "No MQTT service selected"
#endif

// ---------------------------------

#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883

#define MQTT_TOP_TOPIC "iot-course/OleksiiPok"

#define TOPIC_COMMANDS MQTT_TOP_TOPIC "/commands"
#define TOPIC_SENSORS MQTT_TOP_TOPIC "/sensors"
#define TOPIC_TELEMETRY MQTT_TOP_TOPIC "/telemetry"
#define TOPIC_STATUS MQTT_TOP_TOPIC "/status"
// #define TOPIC_ACTUATORS_LED MQTT_TOP_TOPIC "/events"

#define MQTT_RECONNECT_INTERVAL_MS 5000
#define MQTT_MAX_CONNECTION_ATTEMPTS 3
#define MQTT_RECONNECT_CYCLE_DELAY_MS 300000

#define MQTT_BUFFER_SIZE 512

// ---------------------------------
