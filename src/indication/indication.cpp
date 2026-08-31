// src/indication/indication.cpp

#include <Arduino.h>

#include "config.h"
#include "indication.h"

// ---------------------------------
#define LED_BLINK_TIME_MS 50

static const uint8_t LED_PINS[] = {
    LED_LIGHT_MANUAL_PIN, LED_LIGHT_AUTO_PIN,      LED_LIGHT_MIN_PIN,
    LED_LIGHT_MAX_PIN,    LED_TEMPERATURE_MIN_PIN, LED_TEMPERATURE_MAX_PIN,
    LED_HUMIDITY_MIN_PIN, LED_SERIAL_MONITOR_PIN};

static const uint8_t LED_MASKS[] = {LED_LIGHT_MANUAL,    LED_LIGHT_AUTO,
                                    LED_LIGHT_MIN,       LED_LIGHT_MAX,
                                    LED_TEMPERATURE_MIN, LED_TEMPERATURE_MAX,
                                    LED_HUMIDITY_MIN,    LED_SERIAL_MONITOR};
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
  pinMode(LED_SERIAL_MONITOR_PIN, OUTPUT);
}

void handleIndication(uint8_t &ledState) {
  for (uint8_t i = 0; i < sizeof(LED_PINS) / sizeof(LED_PINS[0]); ++i) {
    digitalWrite(LED_PINS[i], (ledState & LED_MASKS[i]) != 0);
  }
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_BLINK_TIME_MS);
  digitalWrite(pin, LOW);
}