// clock.cpp

#include <Arduino.h>
#include <time.h>

#include "clock.h"

// ---------------------------------

// ---------------------------------

void initClock() {
  // Internal system time is UTC (Greenwich).
  configTime(0, 0, NTP_SERVER);
}

time_t getCurrentTimestamp() { return time(nullptr); }

bool getCurrentUtcTime(struct tm &utcTime) {
  time_t now = time(nullptr);

  if (now < 100000) {
    return false;
  }

  gmtime_r(&now, &utcTime);

  return true;
}

bool getLocalTime(time_t utcTimestamp, const char *timezone,
                  struct tm &localTime) {
  if (utcTimestamp < 100000 || timezone == nullptr) {
    return false;
  }

  setenv("TZ", timezone, 1);
  tzset();

  localtime_r(&utcTimestamp, &localTime);

  return true;
}