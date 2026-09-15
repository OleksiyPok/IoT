// src/actions/actions.h

#pragma once

#include <Arduino.h>

// ---------------------------------

void handleActions(const uint16_t &buttonsState, uint16_t &systemState,
                   uint16_t &ledState);
