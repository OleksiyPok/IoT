// src/http/http.h

#pragma once

#include "../telemetry/telemetry.h"

#define SERVER_URL "http://httpbun.com/post" // HTTP POST

void handleSendData(const Telemetry &telemetryData);