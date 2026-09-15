// src/actions/actions.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

// ---------------------------------
void handleActions(const Telemetry &telemetryData, const uint16_t &buttonsState,
                   uint16_t &systemState, uint16_t &ledState);
