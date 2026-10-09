// src/time/time_config.h

#pragma once

static const char *NTP_SERVER_1 = "pool.ntp.org";
static const char *NTP_SERVER_2 = "time.aws.com";
static const char *NTP_SERVER_3 = "europe.pool.ntp.org";

// Set local timezone here.
// Leave undefined to use UTC.
#define LOCAL_TIMEZONE "CET-1CEST,M3.5.0,M10.5.0"
#define DEFAULT_TIMEZONE "UTC"

#ifdef LOCAL_TIMEZONE
static const char *currentTimezone = LOCAL_TIMEZONE;
#else
static const char *currentTimezone = DEFAULT_TIMEZONE;
#endif

#define TIME_SYNC_INTERVAL_MS (TIME_SYNC_INTERVAL_MIN * 60UL * 1000UL)

// ---------------------------------

#define TIME_SYNC_INTERVAL_MIN 5
#define TIME_SYNC_RETRY_INTERVAL_MS 5000
#define TIME_SYNC_TIMEOUT_MS 30000

// ---------------------------------