#include <Arduino.h>

#include "config.h"
#include "indication.h"

// ---------------------------------

struct LedIndication {
  bool active;
  uint8_t pin;
  IndicationType type;
  bool ledOn;
  uint8_t blinkCount;
  uint32_t stateChangedAt;
};

static LedIndication indications[] = {
    {false, LED_BUILTIN_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_COMMAND_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_LIGHT_AUTO_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_LIGHT_MIN_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_LIGHT_MAX_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_TEMPERATURE_MIN_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_TEMPERATURE_MAX_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_HUMIDITY_MIN_PIN, IndicationType::OFF, false, 0, 0},
    {false, LED_SILENT_PIN, IndicationType::OFF, false, 0, 0},
};

static constexpr uint8_t INDICATION_COUNT =
    sizeof(indications) / sizeof(indications[0]);

// ---------------------------------

static LedIndication *findIndication(uint8_t ledPin);
static LedIndication *findFreeIndication();
// static void startIndication(LedIndication &indication);
static void stopIndication(LedIndication &indication);
static uint8_t getBlinkCount(IndicationType type);
static uint32_t getBlinkOnDuration(IndicationType type);
static uint32_t getBlinkOffDuration(IndicationType type);

// ---------------------------------

void initIndication() {
  for (uint8_t i = 0; i < INDICATION_COUNT; ++i) {
    pinMode(indications[i].pin, OUTPUT);
    digitalWrite(indications[i].pin, LOW);
  }
}

// ---------------------------------

void handleIndication() {
  const uint32_t now = millis();

  for (uint8_t i = 0; i < INDICATION_COUNT; ++i) {
    LedIndication &indication = indications[i];

    if (!indication.active) {
      continue;
    }

    if (indication.type == IndicationType::ON) {
      if (!indication.ledOn) {
        indication.ledOn = true;
        digitalWrite(indication.pin, HIGH);
      }

      continue;
    }

    if (indication.type == IndicationType::OFF) {
      continue;
    }

    const uint32_t duration = indication.ledOn
                                  ? getBlinkOnDuration(indication.type)
                                  : getBlinkOffDuration(indication.type);

    if (now - indication.stateChangedAt < duration) {
      continue;
    }

    indication.stateChangedAt = now;

    if (indication.ledOn) {
      indication.ledOn = false;
      digitalWrite(indication.pin, LOW);
      continue;
    }

    indication.blinkCount++;

    if (indication.blinkCount >= getBlinkCount(indication.type)) {
      stopIndication(indication);
      continue;
    }

    indication.ledOn = true;
    digitalWrite(indication.pin, HIGH);
  }
}

// ---------------------------------

void requestIndication(uint8_t ledPin, IndicationType type) {
  LedIndication *indication = findIndication(ledPin);

  if (type == IndicationType::OFF) {
    if (indication != nullptr) {
      stopIndication(*indication);
    }

    return;
  }

  if (indication == nullptr) {
    indication = findFreeIndication();

    if (indication == nullptr) {
      return;
    }

    indication->pin = ledPin;
    pinMode(ledPin, OUTPUT);
  }

  indication->type = type;
  indication->active = true;
  indication->ledOn = true;
  indication->blinkCount = 0;
  indication->stateChangedAt = millis();

  digitalWrite(ledPin, HIGH);
}

// ---------------------------------

static LedIndication *findIndication(uint8_t ledPin) {
  for (uint8_t i = 0; i < INDICATION_COUNT; ++i) {
    if (indications[i].pin == ledPin) {
      return &indications[i];
    }
  }

  return nullptr;
}

static LedIndication *findFreeIndication() {
  for (uint8_t i = 0; i < INDICATION_COUNT; ++i) {
    if (!indications[i].active) {
      return &indications[i];
    }
  }

  return nullptr;
}

// static void startIndication(LedIndication &indication) {
//   indication.active = true;
//   indication.ledOn = true;
//   indication.blinkCount = 0;
//   indication.stateChangedAt = millis();

//   digitalWrite(indication.pin, HIGH);
// }

static void stopIndication(LedIndication &indication) {
  indication.active = false;
  indication.type = IndicationType::OFF;
  indication.ledOn = false;
  indication.blinkCount = 0;

  digitalWrite(indication.pin, LOW);
}

static uint8_t getBlinkCount(IndicationType type) {
  switch (type) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_COUNT;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_COUNT;

  default:
    return 0;
  }
}

static uint32_t getBlinkOnDuration(IndicationType type) {
  switch (type) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_ON_MS;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_ON_MS;

  default:
    return 0;
  }
}

static uint32_t getBlinkOffDuration(IndicationType type) {
  switch (type) {
  case IndicationType::MQTT_PUBLISH:
    return INDICATION_MQTT_PUBLISH_BLINK_OFF_MS;

  case IndicationType::WIFI_CONNECTING:
    return INDICATION_WIFI_CONNECTING_BLINK_OFF_MS;

  default:
    return 0;
  }
}