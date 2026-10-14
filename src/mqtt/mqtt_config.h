// src/mqtt/mqtt_config.h

#pragma once

// ---------------------------------

// Uncomment exactly one service
#define MQTT_SERVICE_AWS
// #define MQTT_SERVICE_TEST

// ---------------------------------

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

#define MQTT_TOP_TOPIC "iot-course/OleksiiPok"

#define MQTT_SUBSCRIBE_QOS 0
#define MQTT_PUBLISH_QOS 0

#define TOPIC_COMMANDS_LED MQTT_TOP_TOPIC "/commands/led"
#define TOPIC_COMMANDS_OUT MQTT_TOP_TOPIC "/commands"
#define TOPIC_TELEMETRY MQTT_TOP_TOPIC "/telemetry"
#define TOPIC_EVENTS MQTT_TOP_TOPIC "/events"

#define MQTT_SOCKET_TIMEOUT_SEC 30
#define MQTT_KEEP_ALIVE 60
#define MQTT_RECONNECT_INTERVAL_MS 15000
#define MQTT_MAX_CONNECTION_ATTEMPTS 3
#define MQTT_RECONNECT_CYCLE_DELAY_MS 300000

#define MQTT_BUFFER_SIZE 512

// ---------------------------------

#if MQTT_PUBLISH_QOS != 0
#error "PubSubClient supports only QoS 0 for publishing"
#endif

#if MQTT_SUBSCRIBE_QOS < 0 || MQTT_SUBSCRIBE_QOS > 1
#error "MQTT_SUBSCRIBE_QOS must be 0 or 1"
#endif
// ---------------------------------
