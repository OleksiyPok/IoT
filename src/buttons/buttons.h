// src/buttons/buttons.h

#pragma once

#define DEBOUNCE 50

void initButtons();
void handleButtons(unsigned char &buttonsState);