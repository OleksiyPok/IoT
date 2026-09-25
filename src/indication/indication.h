// src/indication/indication.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

// ---------------------------------

enum class IndicationType : uint8_t { NONE, MQTT_PUBLISH, WIFI_CONNECTING };

enum class IndicationMode : uint8_t { CONTINUOUS, BLINK };

struct IndicationPattern {
  IndicationMode mode;
  uint8_t blinkCount;
  uint32_t onMs;
  uint32_t offMs;
};

struct LedIndicationState {
  uint16_t mask;
  uint8_t pin;
  IndicationPattern pattern;

  uint8_t currentBlink;
  bool active;
  bool outputOn;
  uint32_t stateChangedAt;
};

#define INDICATION_STARTUP_BLINK_ON_MS 100

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

#define ALL_LED_MASK                                                           \
  (LED_LIGHT_MIN_MASK | LED_LIGHT_MAX_MASK | LED_LIGHT_AUTO_MASK |             \
   LED_TEMPERATURE_MIN_MASK | LED_TEMPERATURE_MAX_MASK |                       \
   LED_HUMIDITY_MIN_MASK | LED_HUMIDITY_MAX_MASK | LED_SILENT_MASK |           \
   LED_COMMAND_MASK)

// ---------------------------------

void initIndication();
void startStartupBlink();

void requestIndication(IndicationType type);

void handleLedIndication(const uint16_t &ledState);
