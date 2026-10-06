// src/mqtt/mqtt_test_service.h

#pragma once

#include "mqtt_service.h"

class MqttTestService : public MqttService {
public:
  void init() override;
  bool isConnected() override;
  bool connect(const char *clientId) override;
  void disconnect() override;
  void loop() override;
  bool publish(const char *topic, const char *payload) override;
  bool subscribe(const char *topic) override;
  void setCallback(MqttMessageCallback callback) override;
  MqttStatus status() override;
};