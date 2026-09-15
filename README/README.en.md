# IoT Project

[🇬🇧 English](./README.en.md) | [🇺🇦 Українська](./README.uk.md)

<img src="../images/wokwi-B.png" alt="Project Circuit" width="700">

## Project Description

The project is an ESP32 firmware organized into independent functional modules.

From the user's perspective, the device:

- connects to WiFi and automatically attempts to restore the connection after a disconnect;
- connects to an MQTT broker and automatically manages the MQTT connection;
- subscribes to command and sensor-data topics;
- receives commands through MQTT and executes supported device actions;
- receives temperature and light data through MQTT and uses them for device indication;
- provides manual light control through a button;
- provides a silent mode;
- provides a WiFi disconnect/recovery test function;
- controls LEDs according to commands, operating conditions, and alarm states;
- synchronizes system time using NTP and provides UTC/local time access;
- provides device identification information;
- optionally outputs diagnostic information to the Serial Monitor in `DEBUG_MODE`.

Temperature and light alarm indication is controlled by compile-time configuration:

- `DHT_TEMPERATURE_ALARM_MIN_CONFIG`
- `DHT_TEMPERATURE_ALARM_MAX_CONFIG`
- `LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG`

The device state is represented by bit registers, allowing several independent conditions to be active simultaneously.

## Main Operation Flow

```text
Buttons ───────────────┐
                       ├──> Actions ──> System State ──> LED Indication
MQTT Commands ─────────┤
                       │
MQTT Sensor Data ──────┘

WiFi ──> MQTT Connection
NTP  ──> System Time

DEBUG_MODE ──> Serial Monitor
```

## Communication

MQTT is used for two-way device communication:

- commands are received from the MQTT command topic;
- sensor data is received from the MQTT sensor topic;
- received data is deserialized and stored for further processing;
- MQTT connection and topic subscriptions are restored automatically after reconnection.

## Buttons and Indication

Four buttons are supported:

- manual command control;
- silent mode;
- additional button input;
- WiFi disconnect/recovery testing.

The LEDs indicate command activity, light conditions, temperature conditions, humidity conditions, and silent mode.

Button inputs use interrupts and software debounce handling.

## Time and Device Information

The firmware initializes system time through NTP and supports:

- current UTC timestamp;
- UTC time structure;
- local time using the configured timezone;
- runtime timezone changes.

The firmware can also obtain the ESP32 WiFi STA device identifier and display the device information.

## Configuration

Operating intervals, WiFi parameters, MQTT parameters, GPIO assignments, LED/button mappings, and alarm thresholds are configured at compile time.

The firmware is designed as a modular system, with separate modules for buttons, actions, commands, indication, WiFi, MQTT, serialization, time, device information, system state, memory, and monitoring.

[🇬🇧 English](./README.en.md) | [🇺🇦 Українська](./README.uk.md)
