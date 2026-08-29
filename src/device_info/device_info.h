// src/device_info/device_info.h

#pragma once

#include "../telemetry/telemetry.h"
#include <Arduino.h>

bool getDeviceId(uint64_t &deviceId);
void printDeviceInfo();