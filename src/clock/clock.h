// src/clock/clock.h

#pragma once

#include <time.h>

// Initialize system clock and synchronize with NTP.
void initClock();

// Get current UTC timestamp.
time_t getCurrentTimestamp();

// Get current UTC time.
bool getCurrentUtcTime(struct tm &utcTime);

// Get current local time using the configured timezone.
bool getLocalTime(struct tm &localTime);

// Set timezone.
void setTimezone(const char *timezone);