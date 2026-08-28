// src/buttons/buttons.h

#pragma once

#include <Arduino.h>

#define DEBOUNCE 50

void initButtons();
void handleButtons(uint8_t &buttonsState);