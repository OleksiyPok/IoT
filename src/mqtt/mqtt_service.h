// src/mqtt/mqtt_service.h

#pragma once

#include <Arduino.h>

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
  virtual bool connect() = 0;
  virtual void disconnect() = 0;
  virtual void loop() = 0;
  virtual bool publish(const char *topic, const char *payload) = 0;
  virtual MqttStatus status() = 0;
};