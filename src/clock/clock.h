// clock.h

#pragma once

#include <time.h>

#define NTP_SERVER "pool.ntp.org"

// Initialize system clock and synchronize with NTP.
void initClock();

// Get current UTC timestamp.
time_t getCurrentTimestamp();

// Convert UTC timestamp to local time according to the specified timezone.
bool getLocalTime(time_t utcTimestamp, const char *timezone,
                  struct tm &localTime);

// Get current UTC time.
bool getCurrentUtcTime(struct tm &utcTime);
