[🇬🇧 English](./README.en.md) | [🇺🇦 Українська](./README.uk.md)

# IoT Project

<img src="../images/wokwi-A.png" alt="Project Circuit" width="700">

## Project Description

ESP32 firmware for environmental monitoring, local indication, and remote control through MQTT. The firmware reads a DHT sensor and an LDR, validates measurements, maintains sensor and system status, synchronizes time with NTP, and exchanges telemetry, events, and commands with an MQTT broker.

The code is split into modules so sensor handling, state aggregation, actions, indication, networking, and JSON serialization remain separate.

## Features

- DHT temperature and humidity measurement.
- LDR ADC measurement and conversion to illuminance in lux.
- Sensor reading validation, alarm flags, device-error flags, and stale-data detection.
- Aggregated system-state register for sensor and connection errors.
- Wi-Fi connection handling, reconnection attempts, and NTP synchronization.
- MQTT connection to AWS IoT Core using TLS and device certificates, or to a public test broker selected in configuration.
- Periodic telemetry publication, status serialization support, outgoing command publication, incoming LED commands, and LED-change events.
- LED indication for light-control mode, light thresholds, temperature/humidity alarms, silent mode, command activity, Wi-Fi connection, and MQTT publication.
- Serial Monitor diagnostics, including bit-level status output and formatted MQTT payloads when debug mode is enabled.

## Hardware and Buttons

Pin assignments are in `src/hardware_config.h`. Check them against the wiring before uploading.

| Button | Default pin | Behaviour |
|---|---:|---|
| Button 0 | `BUTTON_0_PIN` | Momentary action: sets the outgoing command `manual_trigger` and attempts to publish it. |
| Button 1 | `BUTTON_1_PIN` | Fixed/toggle state: enables or disables silent mode. DHT sampling is paused while silent mode is active; LDR sampling continues. |
| Button 2 | `BUTTON_2_PIN` | Fixed/toggle state: enables or disables manual light mode. |
| Button 3 | `BUTTON_3_PIN` | Momentary test action: disconnects Wi-Fi. The normal connection manager may later reconnect. |

The button pins are currently configured as `18`, `19`, `5`, and `4`, respectively. `BUTTON_0_FIXED` through `BUTTON_3_FIXED` in `src/buttons/buttons_config.h` define whether a button state is momentary or retained. Debounce timing is configured there as well.

## LED Indication

LED pins are configured in `src/hardware_config.h`; indication patterns are configured in `src/indication/indication_config.h`.

- **Manual light** — reflects the manual light-control state.
- **Automatic/light-low indication** — reflects the LDR light-low status.
- **Light minimum/maximum** — indicates the configured lux alarm states.
- **Temperature minimum/maximum** — indicates DHT temperature alarm states.
- **Humidity minimum/maximum** — corresponding indication states are supported in the code; physical pins may be left undefined in the hardware configuration.
- **Silent mode** — indicates the silent-mode state.
- **Command** — indicates command activity.
- **Built-in LED** — used for Wi-Fi connection and MQTT publication patterns.

Each indication can be configured as continuous or blinking, with its own blink count and on/off intervals.

## Telemetry Model

The common `Telemetry` structure is defined in `src/telemetry/telemetry.h`. It contains:

- protocol version, device ID, Unix timestamp, and uptime;
- DHT data: temperature, humidity, last successful update time, update uptime, and status flags;
- LDR data: raw ADC value, calculated lux, last successful update time, update uptime, and status flags;
- button state, aggregated system state, and LED indication state.

The telemetry update routine refreshes the timestamp and uptime. Sensor data and system state are updated by their respective modules.

## Sensor Limits and Thresholds

Threshold values are defined symbolically in the sensor configuration files. Adjust them for the sensor, circuit, and deployment; do not hard-code thresholds in application logic.

**DHT** — `src/dht_sensor/dht_config.h`

- `DHT_TEMPERATURE_VALID_MIN`, `DHT_TEMPERATURE_VALID_MAX`
- `DHT_HUMIDITY_VALID_MIN`, `DHT_HUMIDITY_VALID_MAX`
- `DHT_TEMPERATURE_ALARM_MIN`, `DHT_TEMPERATURE_ALARM_MAX`
- `DHT_HUMIDITY_ALARM_MIN`, `DHT_HUMIDITY_ALARM_MAX`

**LDR** — `src/ldr_sensor/ldr_config.h`

- `LDR_ADC_VALID_MIN`, `LDR_ADC_VALID_MAX`
- `LDR_LUX_VALID_MIN`, `LDR_LUX_VALID_MAX`
- `LDR_LUX_ALARM_MIN`, `LDR_LUX_ALARM_MAX`
- `LDR_LUX_THRESHOLD_LIGHT_LOW`

Valid limits determine whether a reading is acceptable. Alarm thresholds set alarm/status flags and drive LED indications; an alarm does not automatically mean a sensor read error. Device/read errors and invalid data are represented separately in the sensor status.

## System State, Status, and STALE

### System state register

`systemState` is a bit field managed by `src/system/system_state.cpp`. It combines user-operating states and communication/sensor error flags, including:

- `SILENT` — silent mode;
- `LDR_ERR`, `DHT_ERR` — sensor errors;
- `TIME_ERR` — time synchronization error;
- `MQTT_ERR`, `WIFI_ERR` — communication errors;
- `LIGHT_MANUAL` — manual light-control mode;
- `COMMAND` — command activity.

Error flags are initialized as active and cleared only when the corresponding condition is confirmed to be normal. The register is recalculated by the system-state module; sensor-specific status remains in the DHT and LDR data structures.

### Sensor status and STALE

DHT and LDR maintain separate status bit fields. These include device/read errors, invalid data, stale data, and their sensor-specific alarm states.

The stale watchdog periodically marks both sensor datasets as `STALE`. A successful new reading clears the relevant stale flag. If a new reading is not successfully confirmed, the stale state remains active. The watchdog interval is derived from the sensor/telemetry intervals in `src/app_config.h`.

## MQTT Communication

The selected service is controlled by `src/mqtt/mqtt_config.h`. Exactly one service must be enabled:

- `MQTT_SERVICE_AWS` — AWS IoT Core over TLS;
- `MQTT_SERVICE_TEST` — public test broker, configured in `src/secrets.h`.

The current topic prefix is `MQTT_TOP_TOPIC` (`iot-course/OleksiiPok`). Topics are derived from that prefix:

| Purpose | Topic |
|---|---|
| Telemetry and status destination | `.../telemetry` |
| Events | `.../events` |
| Outgoing commands | `.../commands` |
| Incoming command subscription | `.../commands/#` |

Change `MQTT_TOP_TOPIC` to match the topics authorized by the AWS IoT policy and used by your AWS IoT Rules/backend.

### Message types

- **Telemetry** — JSON containing protocol version, device ID, timestamp, uptime, sequence, DHT readings/status, LDR readings/status, and `sys_state`.
- **Status** — serializer for common device fields and DHT, LDR, and system status.
- **Outgoing command** — JSON command message. Button 0 currently triggers `manual_trigger`.
- **Incoming command** — JSON commands are parsed and mapped to LED set operations (`on`, `off`, or `toggle`).
- **Event** — `led_changed` is published when the manual-light LED state changes, including changes initiated locally or by an incoming MQTT command. The event includes a timestamp and `value` (`on`/`off`).

The MQTT publishing layer uses QoS 0 for publication because the configured PubSubClient implementation supports QoS 0 publishing. Subscription QoS is configured separately. Telemetry publication cadence and reconnection behaviour are configurable.

## Configuration Reference

| File | What to configure |
|---|---|
| `src/secrets.h` (created from `src/secrets_example.h`) | Wi-Fi credentials, AWS endpoint and certificates, and test-broker settings. Keep real secrets out of version control. |
| `src/app_config.h` | Sensor read intervals, telemetry update/stale timing, MQTT publication cadence, and diagnostic/memory-check intervals. |
| `src/hardware_config.h` | DHT/LDR pins, button pins, and LED pins for the actual circuit. |
| `src/project_config.h` | `DEBUG_MODE` and the optional ArduinoJson telemetry serializer selection. |
| `src/buttons/buttons_config.h` | Button debounce and fixed/momentary behaviour. |
| `src/dht_sensor/dht_config.h` | DHT valid limits and temperature/humidity alarm thresholds. |
| `src/ldr_sensor/ldr_config.h` | ADC/lux valid limits and illumination alarm/light-low thresholds. |
| `src/mqtt/mqtt_config.h` | MQTT service, topic prefix, QoS, socket timeout, keep-alive, reconnect timing/attempts, and buffer size. |
| `src/wifi/wifi_config.h` | Wi-Fi timeout, reconnect interval, attempt count, and retry-cycle delay. |
| `src/time/time_config.h` | NTP servers, local timezone, synchronization interval, retry interval, and timeout. |
| `src/indication/indication_config.h` | LED indication mode and blink patterns. |
| `src/commands_out/commands_out_config.h` | Outgoing command names. |
| `src/serialization/serialization_config.h` | JSON serialization/deserialization options. |

## Build and Run

1. Open the complete project in VS Code with PlatformIO and select the configured ESP32 environment.
2. Create `src/secrets.h` from `src/secrets_example.h`.
3. Fill in the Wi-Fi credentials and select the intended MQTT service.
4. For AWS IoT Core, set `AWS_IOT_ENDPOINT`, `AWS_CERT_CA`, `AWS_CERT_CRT`, and `AWS_CERT_PRIVATE`. Use the CA certificate, device certificate, and matching private key for the registered AWS IoT Thing/device.
5. Set `MQTT_TOP_TOPIC` to the prefix expected by the AWS IoT policy and rules. Ensure the device client ID/Thing name, certificate permissions, endpoint, topic ARNs, IoT Rules, and backend/DynamoDB schema agree.
6. Confirm hardware pins and sensor thresholds, then build and upload the firmware.
7. Open Serial Monitor at **115200 baud**. With `DEBUG_MODE` enabled, inspect sensor status, system-state bits, connection messages, and published JSON.

The example secrets file is a template: replace every placeholder with valid values. Never commit `secrets.h`, private keys, or production credentials.

## Project Structure

- `src/main.cpp` — initialization and periodic non-blocking application loop.
- `src/app_config.h` — application intervals.
- `src/hardware_config.h` — hardware pin mapping.
- `src/project_config.h` — debug and serializer build options.
- `src/secrets_example.h`, `src/secrets.h` — credentials template and local secrets.
- `src/dht_sensor/` — DHT acquisition, validation, alarms, and status.
- `src/ldr_sensor/` — ADC acquisition, lux calculation, validation, alarms, and status.
- `src/telemetry/` — common telemetry model, timestamp/uptime, and stale watchdog.
- `src/system/` — system-state register and aggregated error flags.
- `src/buttons/` — button interrupts, debouncing, and button state.
- `src/actions/` — command triggers and mapping system/sensor state to LED state.
- `src/indication/` — LED outputs and indication patterns.
- `src/commands_in/`, `src/commands_out/` — incoming and outgoing command state.
- `src/events/` — change detection and LED-change event publishing.
- `src/wifi/` — Wi-Fi connection management.
- `src/time/` — NTP synchronization, timestamps, and timezone.
- `src/mqtt/` — MQTT service selection, connection/reconnection, publishing, receiving, and topic configuration.
- `src/serialization/` — JSON serializers and command/telemetry deserializers.
- `src/monitor/` — Serial Monitor diagnostics and payload formatting.
- `src/device_info/` — device identification.
- `src/http/` — HTTP support.
- `src/aws/` — AWS-related support.
- `src/memory/` — memory diagnostics support.

---
[🇬🇧 English](./README.en.md) | [🇺🇦 Українська](./README.uk.md)
