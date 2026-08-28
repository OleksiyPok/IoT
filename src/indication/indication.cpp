// src/indication/indication.cpp

#include "indication.h"
#include "bit_utils.h"
#include "config.h"
#include <Arduino.h>

// ---------------------------------

// ---------------------------------

void initIndication() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
}

void handleIndication(unsigned char &ledState) {
  if (readBit(ledState, 0)) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_BLINK_TIME_MS);
  digitalWrite(pin, LOW);
}