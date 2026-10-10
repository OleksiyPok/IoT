// src/events/events.cpp

#include "events.h"

#include "../indication/indication.h"
#include "../mqtt/mqtt.h"
#include "../mqtt/mqtt_publish.h"

static bool ledStateInitialized = false;
static bool previousLedState = false;

static bool pendingLedEvent = false;
static bool pendingLedState = false;

void handleEvents(const Telemetry &telemetry) {
  const bool currentLedState =
      (telemetry.ledState & INDICATION_LIGHT_MANUAL_MASK) != 0;

  if (!ledStateInitialized) {
    previousLedState = currentLedState;
    ledStateInitialized = true;
    return;
  }

  if (currentLedState != previousLedState) {
    previousLedState = currentLedState;
    pendingLedState = currentLedState;
    pendingLedEvent = true;
  }

  if (!pendingLedEvent || !isMqttConnected()) {
    return;
  }

  if (publishLedChanged(pendingLedState)) {
    pendingLedEvent = false;
  }
}