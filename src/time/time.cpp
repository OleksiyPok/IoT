// src/time/time.cpp

#include <Arduino.h>
#include <time.h>

#include "../config.h"
#include "time.h"

// ---------------------------------
static const char *NTP_SERVER = "pool.ntp.org";

#ifdef LOCAL_TIMEZONE
static const char *currentTimezone = LOCAL_TIMEZONE;
#else
static const char *currentTimezone = DEFAULT_TIMEZONE;
#endif

static bool waitForTimeSync();
static void printCurrentTime();

// ---------------------------------

void initTime() {

  // Internal system time is UTC.
  configTime(0, 0, NTP_SERVER);

  // Set timezone for local time conversion.
  setenv("TZ", currentTimezone, 1);
  tzset();

  Serial.print("[NTP] Time synchronization... ");

  if (!waitForTimeSync()) {
    Serial.println("FAILED");
    return;
  }

  Serial.println("OK");

  printCurrentTime();
}

void syncTime() { configTime(0, 0, NTP_SERVER); }

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

static bool waitForTimeSync() {
  time_t now = time(nullptr);
  const unsigned long startTime = millis();

  while (now < 100000) {
    if (millis() - startTime >= 10000) {
      return false;
    }
    delay(50);
    now = time(nullptr);
  }

  return true;
}

static void printCurrentTime() {

  struct tm utcTime;
  struct tm localTime;

  if (getCurrentUtcTime(utcTime)) {

    Serial.printf("[NTP] UTC: %04d-%02d-%02d %02d:%02d:%02d\r\n",
                  utcTime.tm_year + 1900, utcTime.tm_mon + 1, utcTime.tm_mday,
                  utcTime.tm_hour, utcTime.tm_min, utcTime.tm_sec);
  }

  if (getLocalTime(localTime)) {

    Serial.printf("[NTP] Local: %04d-%02d-%02d %02d:%02d:%02d\r\n",
                  localTime.tm_year + 1900, localTime.tm_mon + 1,
                  localTime.tm_mday, localTime.tm_hour, localTime.tm_min,
                  localTime.tm_sec);
  }
}