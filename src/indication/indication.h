// src/indication/indication.h

#pragma once

#include <Arduino.h>

extern uint8_t ledState; // LED state register (8 leds)
// "ledState" bits
#define LED_LIGHT_MANUAL (1U << 0)
#define LED_LIGHT_AUTO (1U << 1)
#define LED_LIGHT_MIN (1U << 2)
#define LED_LIGHT_MAX (1U << 3)
#define LED_TEMPERATURE_MIN (1U << 4)
#define LED_TEMPERATURE_MAX (1U << 5)
#define LED_HUMIDITY_MIN (1U << 6)
// #define LED_HUMIDITY_MIN (1U << 7)
#define LED_SERIAL_MONITOR (1U << 7)

void initIndication();
void handleIndication(uint8_t &ledState);

void blinkLed(int pin);
