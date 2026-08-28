// src/indication/indication.cpp

#include "indication.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

// ---------------------------------

void initIndication() {
  pinMode(LED_BUILTIN_PIN, OUTPUT);

  pinMode(LED_LIGHT_MANUAL_PIN, OUTPUT);
  pinMode(LED_LIGHT_AUTO_PIN, OUTPUT);
  pinMode(LED_LIGHT_MIN_PIN, OUTPUT);
  pinMode(LED_LIGHT_MAX_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MIN_PIN, OUTPUT);
  pinMode(LED_TEMPERATURE_MAX_PIN, OUTPUT);
}

void handleIndication(uint8_t &ledState) {
  if (isBitSet(ledState, 0)) {
    digitalWrite(LED_LIGHT_MANUAL_PIN, HIGH);
  } else {
    digitalWrite(LED_LIGHT_MANUAL_PIN, LOW);
  }
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_BLINK_TIME_MS);
  digitalWrite(pin, LOW);
}