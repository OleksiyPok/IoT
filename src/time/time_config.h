// src/time/time_config.h

#pragma once

static const char *NTP_SERVER = "pool.ntp.org";

// Set local timezone here.
// Leave undefined to use UTC.
#define LOCAL_TIMEZONE "CET-1CEST,M3.5.0,M10.5.0"
#define DEFAULT_TIMEZONE "UTC"

#ifdef LOCAL_TIMEZONE
static const char *currentTimezone = LOCAL_TIMEZONE;
#else
static const char *currentTimezone = DEFAULT_TIMEZONE;
#endif

#define TIME_SYNC_INTERVAL_MS (TIME_SYNC_INTERVAL_HOURS * 60UL * 60UL * 1000UL)

// ---------------------------------

#define TIME_SYNC_INTERVAL_HOURS 24
#define TIME_SYNC_RETRY_INTERVAL_MS 5000
#define TIME_SYNC_TIMEOUT_MS 5000

// ---------------------------------