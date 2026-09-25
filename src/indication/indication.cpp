// src/indication/indication.cpp

#include <Arduino.h>

#include "../telemetry/telemetry.h"
#include "config.h"
#include "indication.h"
#include "indication_config.h"

// ---------------------------------

static LedIndicationState ledStates[] = {
    {LED_COMMAND_MASK,
     LED_COMMAND_PIN,
     {LED_COMMAND_MODE, LED_COMMAND_BLINK_COUNT, LED_COMMAND_BLINK_ON_MS,
      LED_COMMAND_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_LIGHT_AUTO_MASK,
     LED_LIGHT_AUTO_PIN,
     {LED_LIGHT_AUTO_MODE, LED_LIGHT_AUTO_BLINK_COUNT,
      LED_LIGHT_AUTO_BLINK_ON_MS, LED_LIGHT_AUTO_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_LIGHT_MIN_MASK,
     LED_LIGHT_MIN_PIN,
     {LED_LIGHT_MIN_MODE, LED_LIGHT_MIN_BLINK_COUNT, LED_LIGHT_MIN_BLINK_ON_MS,
      LED_LIGHT_MIN_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_LIGHT_MAX_MASK,
     LED_LIGHT_MAX_PIN,
     {LED_LIGHT_MAX_MODE, LED_LIGHT_MAX_BLINK_COUNT, LED_LIGHT_MAX_BLINK_ON_MS,
      LED_LIGHT_MAX_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_TEMPERATURE_MIN_MASK,
     LED_TEMPERATURE_MIN_PIN,
     {LED_TEMPERATURE_MIN_MODE, LED_TEMPERATURE_MIN_BLINK_COUNT,
      LED_TEMPERATURE_MIN_BLINK_ON_MS, LED_TEMPERATURE_MIN_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_TEMPERATURE_MAX_MASK,
     LED_TEMPERATURE_MAX_PIN,
     {LED_TEMPERATURE_MAX_MODE, LED_TEMPERATURE_MAX_BLINK_COUNT,
      LED_TEMPERATURE_MAX_BLINK_ON_MS, LED_TEMPERATURE_MAX_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_HUMIDITY_MIN_MASK,
     LED_HUMIDITY_MIN_PIN,
     {LED_HUMIDITY_MIN_MODE, LED_HUMIDITY_MIN_BLINK_COUNT,
      LED_HUMIDITY_MIN_BLINK_ON_MS, LED_HUMIDITY_MIN_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_HUMIDITY_MAX_MASK,
     LED_HUMIDITY_MAX_PIN,
     {LED_HUMIDITY_MAX_MODE, LED_HUMIDITY_MAX_BLINK_COUNT,
      LED_HUMIDITY_MAX_BLINK_ON_MS, LED_HUMIDITY_MAX_BLINK_OFF_MS},
     0,
     false,
     false,
     0},

    {LED_SILENT_MASK,
     LED_SILENT_PIN,
     {LED_SILENT_MODE, LED_SILENT_BLINK_COUNT, LED_SILENT_BLINK_ON_MS,
      LED_SILENT_BLINK_OFF_MS},
     0,
     false,
     false,
     0},
};

// ---------------------------------

static bool startupBlinkActive = false;
static uint32_t startupBlinkStartedAt = 0;

static constexpr uint8_t LED_STATE_COUNT =
    sizeof(ledStates) / sizeof(ledStates[0]);

static void resetLedState(LedIndicationState &ledState);
static void startLedIndication(LedIndicationState &ledState, uint32_t now);
static void handleLedIndicationState(LedIndicationState &ledState,
                                     const uint16_t &ledStateMask,
                                     uint32_t now);

static bool indicationActive = false;
static bool indicationLedOn = false;

static uint8_t indicationBlinkCount = 0;
static uint32_t indicationStateChangedAt = 0;

static IndicationType indicationType = IndicationType::MQTT_PUBLISH;
static uint8_t pendingIndications = 0;

static bool handleStartupBlink();
static bool startPendingIndication();
static uint8_t getBlinkCount();
static uint32_t getBlinkOnDuration();
static uint32_t getBlinkOffDuration();

// ---------------------------------

#define INDICATION_PENDING_MQTT_MASK 0x01
#define INDICATION_PENDING_HTTP_MASK 0x02
#define INDICATION_PENDING_WIFI_MASK 0x04

// ---------------------------------

void initIndication() {
  pinMode(LED_BUILTIN_PIN, OUTPUT);

  pinMode(LED_COMMAND_PIN, OUTPUT);
  pinMode(LED_LIGHT_AUTO_PIN, OUTPUT);
  pinMode(LED_LIGHT_MIN_PIN, OUTPUT);
  pinMode(LED_LIGHT_MAX_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MIN_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MAX_PIN, OUTPUT);
  pinMode(LED_HUMIDITY_MIN_PIN, OUTPUT);
  pinMode(LED_HUMIDITY_MAX_PIN, OUTPUT);
  pinMode(LED_SILENT_PIN, OUTPUT);
}

// ---------------------------------
// System indication

void startStartupBlink() {
  startupBlinkActive = true;
  startupBlinkStartedAt = millis();

  for (uint8_t i = 0; i < LED_STATE_COUNT; ++i) {
    digitalWrite(ledStates[i].pin, HIGH);
  }

  digitalWrite(LED_BUILTIN_PIN, HIGH);
}

void requestIndication(IndicationType type) {
  switch (type) {
  case IndicationType::MQTT_PUBLISH:
    pendingIndications |= INDICATION_PENDING_MQTT_MASK;
    break;

  case IndicationType::WIFI_CONNECTING:
    pendingIndications |= INDICATION_PENDING_WIFI_MASK;
    break;
  }

  if (!indicationActive) {
    startPendingIndication();
  }
}

static void startIndication(IndicationType type) {
  indicationType = type;
  indicationBlinkCount = 0;
  indicationLedOn = true;
  indicationActive = true;
  indicationStateChangedAt = millis();

  digitalWrite(LED_BUILTIN_PIN, HIGH);
}

static bool startPendingIndication() {
  if (pendingIndications & INDICATION_PENDING_WIFI_MASK) {
    pendingIndications &= ~INDICATION_PENDING_WIFI_MASK;
    startIndication(IndicationType::WIFI_CONNECTING);
    return true;
  }

  if (pendingIndications & INDICATION_PENDING_MQTT_MASK) {
    pendingIndications &= ~INDICATION_PENDING_MQTT_MASK;
    startIndication(IndicationType::MQTT_PUBLISH);
    return true;
  }

  return false;
}

static uint8_t getBlinkCount() {
  switch (indicationType) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_COUNT;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_COUNT;
  }

  return 0;
}

static uint32_t getBlinkOnDuration() {
  switch (indicationType) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_ON_MS;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_ON_MS;
  }

  return 0;
}

static uint32_t getBlinkOffDuration() {
  switch (indicationType) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_OFF_MS;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_OFF_MS;
  }

  return 0;
}

//++

static void resetLedState(LedIndicationState &ledState) {
  digitalWrite(ledState.pin, LOW);

  ledState.currentBlink = 0;
  ledState.active = false;
  ledState.outputOn = false;
  ledState.stateChangedAt = 0;
}

static void startLedIndication(LedIndicationState &ledState, uint32_t now) {
  ledState.currentBlink = 0;
  ledState.active = true;
  ledState.outputOn = true;
  ledState.stateChangedAt = now;

  digitalWrite(ledState.pin, HIGH);
}

static void handleLedIndicationState(LedIndicationState &ledState,
                                     const uint16_t &ledStateMask,
                                     uint32_t now) {
  const bool allowed = (ledStateMask & ledState.mask) != 0;

  // CONTINUOUS
  if (ledState.pattern.mode == IndicationMode::CONTINUOUS) {
    if (!allowed) {
      resetLedState(ledState);
      return;
    }

    if (!ledState.outputOn) {
      digitalWrite(ledState.pin, HIGH);
      ledState.outputOn = true;
    }

    return;
  }

  // BLINK: start finite sequence
  if (ledState.pattern.blinkCount > 0 && allowed && !ledState.active) {
    startLedIndication(ledState, now);
    return;
  }

  // BLINK: finite sequence already running
  if (ledState.active && ledState.pattern.blinkCount > 0) {
    const uint32_t duration =
        ledState.outputOn ? ledState.pattern.onMs : ledState.pattern.offMs;

    if (now - ledState.stateChangedAt < duration) {
      return;
    }

    ledState.stateChangedAt = now;

    if (ledState.outputOn) {
      digitalWrite(ledState.pin, LOW);
      ledState.outputOn = false;
      return;
    }

    ledState.currentBlink++;

    if (ledState.currentBlink >= ledState.pattern.blinkCount) {
      resetLedState(ledState);
      return;
    }

    digitalWrite(ledState.pin, HIGH);
    ledState.outputOn = true;
    return;
  }

  // BLINK: infinite sequence
  if (ledState.pattern.blinkCount == 0) {
    if (!allowed) {
      resetLedState(ledState);
      return;
    }

    if (!ledState.active) {
      startLedIndication(ledState, now);
      return;
    }

    const uint32_t duration =
        ledState.outputOn ? ledState.pattern.onMs : ledState.pattern.offMs;

    if (now - ledState.stateChangedAt < duration) {
      return;
    }

    ledState.stateChangedAt = now;

    if (ledState.outputOn) {
      digitalWrite(ledState.pin, LOW);
      ledState.outputOn = false;
    } else {
      digitalWrite(ledState.pin, HIGH);
      ledState.outputOn = true;
    }
  }
}

void handleLedIndication(const uint16_t &ledState) {
  if (handleStartupBlink()) {
    return;
  }

  const uint32_t now = millis();

  for (uint8_t i = 0; i < LED_STATE_COUNT; ++i) {
    handleLedIndicationState(ledStates[i], ledState, now);
  }
}

static bool handleStartupBlink() {
  if (!startupBlinkActive) {
    return false;
  }

  if (millis() - startupBlinkStartedAt < INDICATION_STARTUP_BLINK_ON_MS) {
    return true;
  }

  for (uint8_t i = 0; i < LED_STATE_COUNT; ++i) {
    digitalWrite(ledStates[i].pin, LOW);
  }

  digitalWrite(LED_BUILTIN_PIN, LOW);

  startupBlinkActive = false;

  return false;
}