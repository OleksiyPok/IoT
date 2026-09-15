// src/indication/indication.cpp

#include <Arduino.h>

#include "config.h"
#include "indication.h"

// ---------------------------------

#define MANUAL_BLINK_COUNT 3
#define MANUAL_BLINK_INTERVAL_MS 200

static bool blinkActive = false;
static int blinkPin = -1;
static uint8_t blinkCompleted = 0;
static bool blinkOn = false;
static uint32_t blinkNextChangeAt = 0;

static const uint16_t LED_PINS[] = {
    LED_COMMAND_PIN,      LED_LIGHT_AUTO_PIN,      LED_LIGHT_MIN_PIN,
    LED_LIGHT_MAX_PIN,    LED_TEMPERATURE_MIN_PIN, LED_TEMPERATURE_MAX_PIN,
    LED_HUMIDITY_MIN_PIN, LED_SILENT_PIN};

static const uint16_t LED_MASKS[] = {
    LED_COMMAND_MASK,      LED_LIGHT_AUTO_MASK,      LED_LIGHT_MIN_MASK,
    LED_LIGHT_MAX_MASK,    LED_TEMPERATURE_MIN_MASK, LED_TEMPERATURE_MAX_MASK,
    LED_HUMIDITY_MIN_MASK, LED_SILENT_MASK};

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

void handleIndication(const uint16_t &ledState) {
  const uint32_t now = millis();

  if (blinkActive && static_cast<int>(LED_COMMAND_PIN) == blinkPin &&
      static_cast<int32_t>(now - blinkNextChangeAt) >= 0) {
    blinkOn = !blinkOn;

    if (!blinkOn) {
      blinkCompleted++;

      if (blinkCompleted >= MANUAL_BLINK_COUNT) {
        blinkActive = false;
      }
    }

    if (blinkActive) {
      blinkNextChangeAt = now + MANUAL_BLINK_INTERVAL_MS;
    }
  }

  for (uint16_t i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); ++i) {
    if (blinkActive && static_cast<int>(LED_PINS[i]) == blinkPin) {
      digitalWrite(LED_PINS[i], blinkOn);
    } else {
      digitalWrite(LED_PINS[i], (ledState & LED_MASKS[i]) != 0);
    }
  }

  if (!blinkActive && blinkPin == static_cast<int>(LED_COMMAND_PIN)) {
    digitalWrite(LED_COMMAND_PIN, (ledState & LED_COMMAND_MASK) != 0);
    blinkPin = -1;
  }
}

void blinkLed(int pin) {
  blinkPin = pin;
  blinkActive = true;
  blinkCompleted = 0;
  blinkOn = true;
  blinkNextChangeAt = millis() + MANUAL_BLINK_INTERVAL_MS;

  digitalWrite(pin, HIGH);
}