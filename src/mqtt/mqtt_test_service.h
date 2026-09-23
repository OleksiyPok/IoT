// src/mqtt/mqtt_test_service.h

#pragma once

#include "mqtt_service.h"

class MqttTestService : public MqttService {
public:
  void init() override;
  bool isConnected() override;
  bool connect() override;
  void disconnect() override;
  void loop() override;
  bool publish(const char *topic, const char *payload) override;
  MqttStatus status() override;
};
