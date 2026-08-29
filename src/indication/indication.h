// src/indication/indication.h

#pragma once

#include <Arduino.h>

extern uint8_t ledState; // LED state register (8 leds)
// "ledState" bits
#define LED_LIGHT_MANUAL_BIT 0
#define LED_LIGHT_AUTO_BIT 1
#define LED_LIGHT_MIN_BIT 2
#define LED_LIGHT_MAX_BIT 3
#define LED_TEMPERATURE_MIN_BIT 4
#define LED_TEMPERATURE_MAX_BIT 5

#define LED_LIGHT_MANUAL_MASK 0b00000001
#define LED_LIGHT_AUTO_MASK 0b00000010
#define LED_LIGHT_MIN_MASK 0b00000100
#define LED_LIGHT_MAX_MASK 0b00001000
#define LED_TEMPERATURE_MIN_MASK 0b00010000
#define LED_TEMPERATURE_MAX_MASK 0b00100000

void initIndication();
void handleIndication(uint8_t &ledState);

void blinkLed(int pin);
