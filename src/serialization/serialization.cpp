// src/serialization/serialization.cpp

#include "../config.h"

#include "serialization.h"

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
#include "serializers/telemetry_serializer_aj.h"
#else
#include "serializers/telemetry_serializer.h"
#endif

// ---------------------------------

bool serializeTelemetry(const Telemetry &data, char *buffer,
                        size_t bufferSize) {

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)

  return serializeTelemetryArduinoJson(data, buffer, bufferSize);

#else

  return serializeTelemetrySnprintf(data, buffer, bufferSize);

#endif
}