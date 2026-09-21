// src/indication/indication.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

// ---------------------------------

enum class IndicationType : uint8_t { NONE, MQTT_PUBLISH, WIFI_CONNECTING };

// Built-in LED indication

#define INDICATION_MQTT_PUBLISH_BLINK_COUNT 1
#define INDICATION_MQTT_PUBLISH_BLINK_ON_MS 200
#define INDICATION_MQTT_PUBLISH_BLINK_OFF_MS 400

#define INDICATION_WIFI_CONNECTING_BLINK_COUNT 5
#define INDICATION_WIFI_CONNECTING_BLINK_ON_MS 50
#define INDICATION_WIFI_CONNECTING_BLINK_OFF_MS 50

// ---------------------------------

// "ledState" bits

#define LED_LIGHT_MIN_MASK (1U << 0)
#define LED_LIGHT_MAX_MASK (1U << 1)
#define LED_LIGHT_AUTO_MASK (1U << 2)

#define LED_TEMPERATURE_MIN_MASK (1U << 4)
#define LED_TEMPERATURE_MAX_MASK (1U << 5)
#define LED_HUMIDITY_MIN_MASK (1U << 6)
#define LED_HUMIDITY_MAX_MASK (1U << 7)

#define LED_SILENT_MASK (1U << 8)
#define LED_COMMAND_MASK (1U << 9)

void initIndication();
void handleIndication(const Telemetry &telemetry);
void requestIndication(IndicationType type);
