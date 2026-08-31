// src/device_info/device_info.h

#pragma once

#include <Arduino.h>

#include "../telemetry/telemetry.h"

bool getDeviceId(uint64_t &deviceId);
void printDeviceInfo();