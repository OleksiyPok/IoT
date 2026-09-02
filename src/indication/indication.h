// src/indication/indication.h

#pragma once

#include <Arduino.h>

// "systemState" bits
#define LED_LIGHT_MANUAL_MASK (1U << 0)
#define LED_LIGHT_AUTO_MASK (1U << 1)
#define LED_LIGHT_MIN_MASK (1U << 2)
#define LED_LIGHT_MAX_MASK (1U << 3)
#define LED_TEMPERATURE_MIN (1U << 4)
#define LED_TEMPERATURE_MAX_MASK (1U << 5)
#define LED_HUMIDITY_MIN_MASK (1U << 6)
// #define LED_HUMIDITY_MIN_MASK (1U << 7)
#define LED_SILENT_MASK (1U << 7)

void initIndication();
void handleIndication(uint8_t &systemState);

void blinkLed(int pin);
