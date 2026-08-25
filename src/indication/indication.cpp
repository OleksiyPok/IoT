// src/indication/indication.cpp

#include "indication.h"
#include "config.h"
#include <Arduino.h>


void indication_init() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
};

void blinkLed(int pin) {
  digitalWrite(pin, HIGH);
  delay(LED_ON_TIME_MS);
  digitalWrite(pin, LOW);
}