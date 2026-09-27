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

#define INDICATION_LIGHT_MIN_MASK (1U << 0)
#define INDICATION_LIGHT_MAX_MASK (1U << 1)
#define INDICATION_LIGHT_AUTO_MASK (1U << 2)

#define INDICATION_TEMPERATURE_MIN_MASK (1U << 4)
#define INDICATION_TEMPERATURE_MAX_MASK (1U << 5)
#define INDICATION_HUMIDITY_MIN_MASK (1U << 6)
#define INDICATION_HUMIDITY_MAX_MASK (1U << 7)

#define INDICATION_SILENT_MASK (1U << 8)
#define INDICATION_COMMAND_MASK (1U << 9)

#define ALL_INDICATION_MASK                                                    \
  (INDICATION_LIGHT_MIN_MASK | INDICATION_LIGHT_MAX_MASK |                     \
   INDICATION_LIGHT_AUTO_MASK | INDICATION_TEMPERATURE_MIN_MASK |              \
   INDICATION_TEMPERATURE_MAX_MASK | INDICATION_HUMIDITY_MIN_MASK |            \
   INDICATION_HUMIDITY_MAX_MASK | INDICATION_SILENT_MASK |                     \
   INDICATION_COMMAND_MASK)

// ---------------------------------

void initIndication();
void startStartupBlink();

void requestIndication(IndicationType type);

void handleLedIndication(const uint16_t &ledState);
