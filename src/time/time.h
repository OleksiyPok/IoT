// src/time/time.h

#pragma once

#include <time.h>

// ---------------------------------
#define TIME_SYNC_INTERVAL_MS (TIME_SYNC_INTERVAL_HOURS * 60UL * 60UL * 1000UL)

// ---------------------------------
// Initialize system time and synchronize with NTP.
bool initTime();

// Synchronize system time with NTP server.
void syncTime();

// Get current UTC timestamp.
time_t getCurrentTimestamp();

// Get current UTC time.
bool getCurrentUtcTime(struct tm &utcTime);

// Get current local time using the configured timezone.
bool getLocalTime(struct tm &localTime);

// Set timezone.
void setTimezone(const char *timezone);
