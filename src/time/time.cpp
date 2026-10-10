// src/time/time.cpp

#include <Arduino.h>
#include <time.h>

#include "../project_config.h"
#include "time.h"
#include "time_config.h"

// ---------------------------------

static bool timeSyncPending = false;
static bool timeSynchronized = false;

static uint32_t timeSyncStartedAt = 0;
static uint32_t timeLastAttemptAt = 0;
static uint32_t timeLastSynchronizedAt = 0;

static void startTimeSynchronization();
static void printCurrentTime();

// ---------------------------------

void handleTimeSynchronization() {

  const uint32_t now = millis();

  // An NTP synchronization is currently in progress.
  if (timeSyncPending) {

    const time_t currentTime = time(nullptr);

    // NTP synchronization completed.
    if (currentTime >= 100000) {

      timeSyncPending = false;
      timeSynchronized = true;
      timeLastSynchronizedAt = now;

      printCurrentTime();

      return;
    }

    // Current attempt timed out.
    if (now - timeSyncStartedAt >= NTP_SYNC_TIMEOUT_MS) {

      timeSyncPending = false;
      timeLastAttemptAt = now;

      Serial.println("[NTP] Synchronization timeout");
    }

    return;
  }

  // Initial synchronization or retry after failure.
  if (!timeSynchronized) {

    if (timeLastAttemptAt == 0 ||
        now - timeLastAttemptAt >= NTP_SYNC_RETRY_INTERVAL_MS) {

      startTimeSynchronization();
    }

    return;
  }

  // Periodic synchronization.
  if (now - timeLastSynchronizedAt >= NTP_SYNC_INTERVAL_MS) {

    startTimeSynchronization();
  }
}

bool isTimeSynchronized() { return timeSynchronized; }

void invalidateTimeSync() {
  timeSyncPending = false;
  timeSynchronized = false;

  timeSyncStartedAt = 0;
  timeLastAttemptAt = 0;
  timeLastSynchronizedAt = 0;
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

static void startTimeSynchronization() {

  if (timeSyncPending) {
    return;
  }

  Serial.println("[NTP] Synchronizing time...");

  configTime(0, 0, NTP_SERVER_1, NTP_SERVER_2, NTP_SERVER_3);

  setenv("TZ", currentTimezone, 1);
  tzset();

  const uint32_t now = millis();

  timeSyncPending = true;
  timeSyncStartedAt = now;
  timeLastAttemptAt = now;
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