#pragma once

#include <Arduino.h>

// ---------------------------------

enum class IndicationType : uint8_t { OFF, ON, MQTT_PUBLISH, WIFI_CONNECTING };

// ---------------------------------

#define INDICATION_MQTT_PUBLISH_BLINK_COUNT 1
#define INDICATION_MQTT_PUBLISH_BLINK_ON_MS 200
#define INDICATION_MQTT_PUBLISH_BLINK_OFF_MS 400

#define INDICATION_WIFI_CONNECTING_BLINK_COUNT 5
#define INDICATION_WIFI_CONNECTING_BLINK_ON_MS 50
#define INDICATION_WIFI_CONNECTING_BLINK_OFF_MS 50

// ---------------------------------

void initIndication();

void handleIndication();

void requestIndication(uint8_t ledPin, IndicationType type);