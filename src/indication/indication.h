// src/indication/indication.h

#pragma once

#include <Arduino.h>

#define LED_BLINK_TIME_MS 100

void initIndication();
void handleIndication(uint8_t &ledState);

void blinkLed(int pin);
