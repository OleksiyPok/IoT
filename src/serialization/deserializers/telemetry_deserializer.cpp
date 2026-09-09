// src/serialization/deserializers/telemetry_deserializer.cpp

#include <ArduinoJson.h>

#include "../../telemetry/telemetry.h"
#include "telemetry_deserializer.h"

// ---------------------------------

#define TELEMETRY_JSON_DOCUMENT_SIZE 256

// ---------------------------------

bool deserializeTelemetry(const char *buffer, size_t bufferSize,
                          Telemetry &data) {

  if (buffer == nullptr || bufferSize == 0) {
    return false;
  }

  DynamicJsonDocument doc(TELEMETRY_JSON_DOCUMENT_SIZE);
  DeserializationError error = deserializeJson(doc, buffer, bufferSize);

  if (error) {
    return false;
  }

  // Version
  if (!doc["version"].is<uint8_t>()) {
    return false;
  }

  uint8_t version = doc["version"].as<uint8_t>();

  if (version != TELEMETRY_PROTOCOL_VERSION) {
    return false;
  }

  data.version = version;

  // Root fields
  if (!doc["deviceId"].is<uint64_t>() || !doc["timestamp"].is<uint32_t>() ||
      !doc["uptime"].is<uint32_t>() || !doc["sequence"].is<uint8_t>() ||
      !doc["status"].is<uint8_t>()) {
    return false;
  }

  data.deviceId = doc["deviceId"].as<uint64_t>();
  data.timestamp = doc["timestamp"].as<uint32_t>();
  data.uptime = doc["uptime"].as<uint32_t>();
  data.sequence = doc["sequence"].as<uint8_t>();
  data.status = doc["status"].as<uint8_t>();

  // DHT
  JsonObject dht = doc["dht"];

  if (dht.isNull()) {
    return false;
  }

  if (!dht["temperature"].is<float>() || !dht["humidity"].is<float>() ||
      !dht["updated"].is<uint32_t>() || !dht["status"].is<uint8_t>()) {
    return false;
  }

  data.dht.temperature = dht["temperature"].as<float>();
  data.dht.humidity = dht["humidity"].as<float>();
  data.dht.updated = dht["updated"].as<uint32_t>();
  data.dht.status = dht["status"].as<uint8_t>();

  // LDR
  JsonObject ldr = doc["ldr"];

  if (ldr.isNull()) {
    return false;
  }

  if (!ldr["raw"].is<uint16_t>() || !ldr["lux"].is<float>() ||
      !ldr["updated"].is<uint32_t>() || !ldr["status"].is<uint8_t>()) {
    return false;
  }

  data.ldr.raw = ldr["raw"].as<uint16_t>();
  data.ldr.lux = ldr["lux"].as<float>();
  data.ldr.updated = ldr["updated"].as<uint32_t>();
  data.ldr.status = ldr["status"].as<uint8_t>();

  return true;
}