// src/indication/indication.h

#pragma once

#define LED_BLINK_TIME_MS 100

void initIndication();
void handleIndication(unsigned char *ledState);

void blinkLed(int pin);
