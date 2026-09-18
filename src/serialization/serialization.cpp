// src/serialization/serialization.cpp

#include "../config.h"

#include "serialization.h"

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
#include "serializers/telemetry_serializer_aj.h"
#else
#include "serializers/telemetry_serializer.h"
#endif

#include "serializers/commands_serializer.h"
#include "serializers/sensors_serializer.h"
#include "serializers/status_serializer.h"

// ---------------------------------
bool serializeTelemetry(const Telemetry &data, char *buffer,
                        size_t bufferSize) {

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)

  return serializeTelemetryArduinoJson(data, buffer, bufferSize);

#else

  return serializeTelemetrySnprintf(data, buffer, bufferSize);

#endif
}

bool serializeSensors(const Telemetry &data, char *buffer, size_t bufferSize) {
  return serializeSensorsSnprintf(data, buffer, bufferSize);
};
bool serializeStatus(const Telemetry &data, char *buffer, size_t bufferSize) {
  return serializeStatusSnprintf(data, buffer, bufferSize);
};
bool serializeCommands(const char *commands, char *buffer, size_t bufferSize) {
  return serializeCommandsSnprintf(commands, buffer, bufferSize);
};
