// src/indication/indication.cpp

#include <Arduino.h>

#include "config.h"
#include "indication.h"

// ---------------------------------
#define LED_BLINK_TIME_MS 50

static const uint8_t LED_PINS[] = {
    LED_LIGHT_MANUAL_PIN, LED_LIGHT_AUTO_PIN,      LED_LIGHT_MIN_PIN,
    LED_LIGHT_MAX_PIN,    LED_TEMPERATURE_MIN_PIN, LED_TEMPERATURE_MAX_PIN,
    LED_HUMIDITY_MIN_PIN, LED_SILENT_PIN};

static const uint8_t LED_MASKS[] = {LED_LIGHT_MANUAL_MASK,
                                    LED_LIGHT_AUTO_MASK,
                                    LELED_LIGHT_MIN_MASK LED_LIGHT_MAX_MASK,
                                    LED_TEMPERATURE_MIN,
                                    LED_TEMPERATURE_MAX_MASK,
                                    LED_HUMIDITY_MIN_MASK,
                                    LED_SILENT_MASK};
// ---------------------------------

void initIndication() {
  pinMode(LED_BUILTIN_PIN, OUTPUT);

  pinMode(LED_LIGHT_MANUAL_PIN, OUTPUT);
  pinMode(LED_LIGHT_AUTO_PIN, OUTPUT);
  pinMode(LED_LIGHT_MIN_PIN, OUTPUT);
  pinMode(LED_LIGHT_MAX_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MIN_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MAX_PIN, OUTPUT);
  pinMode(LED_HUMIDITY_MIN_PIN, OUTPUT);
  pinMode(LED_SILENT_PIN, OUTPUT);
}

void handleIndication(uint8_t &systemState) {
  for (uint8_t i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); ++i) {
    digitalWrite(LED_PINS[i], (systemState & LED_MASKS[i]) != 0);
  }
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_BLINK_TIME_MS);
  digitalWrite(pin, LOW);
}