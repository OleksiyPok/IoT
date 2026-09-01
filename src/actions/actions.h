// src/actions/actions.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

void handleActions(const Telemetry &telemetryData, uint8_t &buttonsState,
                   uint8_t &systemState);
