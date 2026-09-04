// src/monitor/monitor_status.h

#pragma once

#include <Arduino.h>

// ---------------------------------

void printTelemetryStatus(const uint8_t &status);
void printLdrStatus(const uint8_t &status);
void printDhtStatus(const uint8_t &status);

void printButtonsState(const uint8_t &state);
void printLedState(const uint8_t &state);