// src/indication/indication.h

#pragma once

#include <Arduino.h>

extern uint8_t ledState; // LED state register (8 leds)
// "ledState" bits
#define LED_LIGHT_MANUAL 0b00000001
#define LED_LIGHT_AUTO 0b00000010
#define LED_LIGHT_MIN 0b00000100
#define LED_LIGHT_MAX 0b00001000
#define LED_TEMPERATURE_MIN 0b00010000
#define LED_TEMPERATURE_MAX 0b00100000
#define LED_HUMIDITY_MIN 0b01000000
#define LED_HUMIDITY_MAX 0b10000000

void initIndication();
void handleIndication(uint8_t &ledState);

void blinkLed(int pin);
