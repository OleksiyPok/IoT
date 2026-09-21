// src/actions/actions.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

// ---------------------------------
void handleActions(Telemetry &telemetry, const uint16_t &buttonsState,
                   uint16_t &systemState);
