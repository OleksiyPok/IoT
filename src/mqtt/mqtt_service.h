// src/mqtt/mqtt_service.h

#pragma once

#include <Arduino.h>

using MqttMessageCallback = void (*)(const char *topic, const uint8_t *payload,
                                     size_t length);

enum class MqttStatus {
  Connected,
  ConnectionTimeout,
  ConnectionLost,
  ConnectFailed,
  Disconnected,
  BadProtocol,
  BadClientId,
  Unavailable,
  BadCredentials,
  Unauthorized,
  Unknown
};

class MqttService {
public:
  virtual ~MqttService() = default;

  virtual void init() = 0;
  virtual bool isConnected() = 0;
  virtual bool connect(const char *clientId) = 0;
  virtual void disconnect() = 0;
  virtual void loop() = 0;
  virtual bool publish(const char *topic, const char *payload) = 0;
  virtual bool subscribe(const char *topic) = 0;
  virtual void setCallback(MqttMessageCallback callback) = 0;
  virtual MqttStatus status() = 0;
};