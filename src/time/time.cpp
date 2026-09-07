// src/time/time.cpp

#include <Arduino.h>
#include <time.h>

#include "time.h"

// ---------------------------------

static const char *NTP_SERVER = "pool.ntp.org";
static const char *DEFAULT_TIMEZONE = "Europe/Amsterdam";
static const char *currentTimezone = DEFAULT_TIMEZONE;

// ---------------------------------

void initTime() {
  // Internal system time is UTC (Greenwich).
  configTime(0, 0, NTP_SERVER);

  // Set default timezone for local time conversion.
  setenv("TZ", currentTimezone, 1);
  tzset();
}

time_t getCurrentTimestamp() { return time(nullptr); }
bool getCurrentUtcTime(struct tm &utcTime) {
  time_t now = time(nullptr);

  // Time has not been synchronized yet.
  if (now < 100000) {
    return false;
  }

  gmtime_r(&now, &utcTime);

  return true;
}

bool getLocalTime(struct tm &localTime) {
  time_t now = time(nullptr);

  // Time has not been synchronized yet.
  if (now < 100000) {
    return false;
  }

  localtime_r(&now, &localTime);

  return true;
}

void setTimezone(const char *timezone) {
  if (timezone == nullptr) {
    return;
  }

  currentTimezone = timezone;

  setenv("TZ", currentTimezone, 1);
  tzset();
}