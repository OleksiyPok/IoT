// src/time/time.h

#pragma once

#include <time.h>

// ---------------------------------

// Request NTP synchronization.
void syncTime();

// Check whether the requested NTP synchronization has completed.
bool isTimeSynchronized();

// Invalidate synchronization after Wi-Fi loss.
void invalidateTimeSync();

// Get current UTC timestamp.
time_t getCurrentTimestamp();

// Get current UTC time.
bool getCurrentUtcTime(struct tm &utcTime);

// Get current local time using the configured timezone.
bool getLocalTime(struct tm &localTime);

// Set timezone.
void setTimezone(const char *timezone);
