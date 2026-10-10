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
#define NTP_SYNC_INTERVAL_MIN 5
// Interval between automatic NTP synchronization attempts, in milliseconds
#define NTP_SYNC_INTERVAL_MS (NTP_SYNC_INTERVAL_MIN * 60UL * 1000UL)
// Minimum interval between retry checks, in milliseconds
#define NTP_SYNC_RETRY_INTERVAL_MS 5000
// Maximum time to wait for an NTP synchronization attempt to complete, in
// milliseconds
#define NTP_SYNC_TIMEOUT_MS 30000

// ---------------------------------

#if NTP_SYNC_INTERVAL_MIN <= 0
#error "NTP_SYNC_INTERVAL_MIN must be greater than 0"
#endif

#if NTP_SYNC_RETRY_INTERVAL_MS <= 0
#error "NTP_SYNC_RETRY_INTERVAL_MS must be greater than 0"
#endif

#if NTP_SYNC_TIMEOUT_MS <= 0
#error "NTP_SYNC_TIMEOUT_MS must be greater than 0"
#endif

#if NTP_SYNC_TIMEOUT_MS >= NTP_SYNC_INTERVAL_MS
#error "NTP_SYNC_TIMEOUT_MS must be less than NTP_SYNC_INTERVAL_MS"
#endif

// ---------------------------------