// src/buttons/buttons.h

#pragma once

#include <Arduino.h>

#define BUTTON_DEBOUNCE_TIME_MS 100

#define BUTTON_LIGHT_MASK (1U << 0)
#define BUTTON_SILENT_MASK (1U << 1)
#define BUTTON_WIFI_DISABLE_MASK (1U << 2)

void initButtons();
void handleButtons(uint8_t &buttonsState);