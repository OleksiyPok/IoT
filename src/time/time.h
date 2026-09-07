// src/time/time.h

#pragma once

#include <time.h>

// Initialize system time and synchronize with NTP.
void initTime();

// Get current UTC timestamp.
time_t getCurrentTimestamp();

// Get current UTC time.
bool getCurrentUtcTime(struct tm &utcTime);

// Get current local time using the configured timezone.
bool getLocalTime(struct tm &localTime);

// Set timezone.
void setTimezone(const char *timezone);