// src/buttons/buttons.h

#pragma once

#include <Arduino.h>

#define DEBOUNCE 100

#define BUTTON_LIGHT_MASK (1U << 0)
#define BUTTON_SERIAL_MONITOR_MASK (1U << 1)
#define BUTTON_WIFI_DISABLE (1U << 2)

void initButtons();
void handleButtons(uint8_t &buttonsState);