// src/indication/indication.cpp

#include "indication.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------
#define LED_BLINK_TIME_MS 100

uint8_t ledState = 0x00;

static const uint8_t LED_PINS[] = {
    LED_LIGHT_MANUAL_PIN, LED_LIGHT_AUTO_PIN,      LED_LIGHT_MIN_PIN,
    LED_LIGHT_MAX_PIN,    LED_TEMPERATURE_MIN_PIN, LED_TEMPERATURE_MAX_PIN};

static const uint8_t LED_MASKS[] = {
    LED_LIGHT_MANUAL_MASK, LED_LIGHT_AUTO_MASK,      LED_LIGHT_MIN_MASK,
    LED_LIGHT_MAX_MASK,    LED_TEMPERATURE_MIN_MASK, LED_TEMPERATURE_MAX_MASK};

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
  for (uint8_t bit = 0; bit < sizeof(LED_PINS); ++bit)
    digitalWrite(LED_PINS[bit], areBitsSetMask(ledState, LED_MASKS[bit]));
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_BLINK_TIME_MS);
  digitalWrite(pin, LOW);
}