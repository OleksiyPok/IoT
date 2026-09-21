// src/indication/indication.cpp

#include <Arduino.h>

#include "../telemetry/telemetry.h"
#include "config.h"
#include "indication.h"

// ---------------------------------

static const uint16_t LED_PINS[] = {
    LED_COMMAND_PIN,      LED_LIGHT_AUTO_PIN,      LED_LIGHT_MIN_PIN,
    LED_LIGHT_MAX_PIN,    LED_TEMPERATURE_MIN_PIN, LED_TEMPERATURE_MAX_PIN,
    LED_HUMIDITY_MIN_PIN, LED_SILENT_PIN};

static const uint16_t LED_MASKS[] = {
    LED_COMMAND_MASK,      LED_LIGHT_AUTO_MASK,      LED_LIGHT_MIN_MASK,
    LED_LIGHT_MAX_MASK,    LED_TEMPERATURE_MIN_MASK, LED_TEMPERATURE_MAX_MASK,
    LED_HUMIDITY_MIN_MASK, LED_SILENT_MASK};

// ---------------------------------

static bool indicationActive = false;
static bool indicationLedOn = false;

static uint8_t indicationBlinkCount = 0;
static uint32_t indicationStateChangedAt = 0;

static IndicationType indicationType = IndicationType::MQTT_PUBLISH;
static uint8_t pendingIndications = 0;

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
  pinMode(LED_SILENT_PIN, OUTPUT);
}

void handleIndication(const Telemetry &telemetry) {
  for (uint16_t i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); ++i) {
    digitalWrite(LED_PINS[i], (telemetry.ledState & LED_MASKS[i]) != 0);
  }

  if (!indicationActive) {
    return;
  }

  const uint32_t now = millis();
  const uint32_t duration =
      indicationLedOn ? getBlinkOnDuration() : getBlinkOffDuration();

  if (now - indicationStateChangedAt < duration) {
    return;
  }

  indicationStateChangedAt = now;

  if (indicationLedOn) {
    indicationLedOn = false;
    digitalWrite(LED_BUILTIN_PIN, LOW);
    return;
  }

  indicationBlinkCount++;

  if (indicationBlinkCount >= getBlinkCount()) {
    indicationActive = false;
    digitalWrite(LED_BUILTIN_PIN, LOW);

    startPendingIndication();
    return;
  }

  indicationLedOn = true;
  digitalWrite(LED_BUILTIN_PIN, HIGH);
}

// ---------------------------------

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