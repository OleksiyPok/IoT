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

// ---------------------------------

// Interval between automatic NTP synchronization attempts, in minutes
#define TIME_SYNC_INTERVAL_MIN 5
// Interval between automatic NTP synchronization attempts, in milliseconds
#define TIME_SYNC_INTERVAL_MS (TIME_SYNC_INTERVAL_MIN * 60UL * 1000UL)
// Minimum interval between retry checks, in milliseconds
#define TIME_SYNC_RETRY_INTERVAL_MS 5000
// Maximum time to wait for an NTP synchronization attempt to complete, in
// milliseconds
#define TIME_SYNC_TIMEOUT_MS 30000

// ---------------------------------

#if TIME_SYNC_INTERVAL_MIN <= 0
#error "TIME_SYNC_INTERVAL_MIN must be greater than 0"
#endif

#if TIME_SYNC_RETRY_INTERVAL_MS <= 0
#error "TIME_SYNC_RETRY_INTERVAL_MS must be greater than 0"
#endif

#if TIME_SYNC_TIMEOUT_MS <= 0
#error "TIME_SYNC_TIMEOUT_MS must be greater than 0"
#endif

#if TIME_SYNC_RETRY_INTERVAL_MS >= TIME_SYNC_TIMEOUT_MS
#error "TIME_SYNC_RETRY_INTERVAL_MS must be less than TIME_SYNC_TIMEOUT_MS"
#endif

#if TIME_SYNC_TIMEOUT_MS >= TIME_SYNC_INTERVAL_MS
#error "TIME_SYNC_TIMEOUT_MS must be less than TIME_SYNC_INTERVAL_MS"
#endif

// ---------------------------------