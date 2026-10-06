// src/serialization/serialization.cpp

#include "../project_config.h"

#include "serialization.h"

#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
#include "serializers/telemetry_serializer_aj.h"
#else
#include "serializers/telemetry_serializer.h"
#endif

#include "deserializers/commands_deserializer.h"
#include "serializers/commands_serializer.h"
#include "serializers/status_serializer.h"


// ---------------------------------
bool serializeTelemetry(const Telemetry &telemetry, uint8_t sequence,
                        char *buffer, size_t bufferSize) {
#if defined(TELEMETRY_SERIALIZER_ARDUINO_JSON)
  return serializeTelemetryArduinoJson(telemetry, sequence, buffer, bufferSize);
#else
  return serializeTelemetrySnprintf(telemetry, sequence, buffer, bufferSize);
#endif
}

bool serializeStatus(const Telemetry &telemetry, uint8_t sequence, char *buffer,
                     size_t bufferSize) {
  return serializeStatusSnprintf(telemetry, sequence, buffer, bufferSize);
};

bool serializeCommands(const char *commands, char *buffer, size_t bufferSize) {
  return serializeCommandsSnprintf(commands, buffer, bufferSize);
};
