# IoT Project

[🇬🇧 English](./README.en.md) | [🇺🇦 Українська](./README.uk.md)

<img src="../images/wokwi.png" alt="Project Circuit" width="700">

## Project Description

The project is an ESP32 firmware organized into independent functional modules.

The main modules are responsible for:

- sensor data acquisition and validation;
- telemetry and status processing;
- button handling and command queuing;
- LED indication;
- WiFi and MQTT communication;
- JSON serialization and deserialization;
- Serial Monitor output.

The main measurement flow is:

```text
DHT22 ──> Temperature ──┐
          Humidity ─────┤
                        ├──> Validation ──> Sensor Status ──┐
LDR ──> ADC ──> Lux ────┘                                   │
                                                            ├──> System State
                                                            ├──> LED indication
                                                            ├──> MQTT
                                                            └──> Serial Monitor
```

Each sensor value is processed through validation limits and operating/alarm thresholds. Sensor data also contains its own update timestamp and uptime.

## Data Structure

The internal telemetry contains the protocol version, device information, time data, sensor measurements, sensor status registers, and system status.

```cpp
struct DHTData {
  float temperature; // °C
  float humidity;    // %
  uint32_t updated;
  uint32_t uptime;
  uint16_t status;
};

struct LDRData {
  uint16_t raw; // ADC data (0–4095)
  float lux;    // lux
  uint32_t updated;
  uint32_t uptime;
  uint16_t status;
};

struct Telemetry {
  uint8_t version; // protocol version
  uint64_t deviceId;
  uint32_t timestamp;
  uint32_t uptime;
  uint8_t sequence;
  DHTData dht;
  LDRData ldr;
  uint16_t status; // system status register
};
```

The telemetry protocol version is currently `1`.

The sequence number is an 8-bit counter and wraps automatically after `255`.

## Measurement Structure

### Temperature

| Parameter | Value |
|---|---:|
| Valid minimum | −35 °C (`DHT_TEMPERATURE_VALID_MIN`) |
| Alarm minimum | 20 °C (`DHT_TEMPERATURE_ALARM_MIN_CONFIG`) |
| Alarm maximum | 26 °C (`DHT_TEMPERATURE_ALARM_MAX_CONFIG`) |
| Valid maximum | +75 °C (`DHT_TEMPERATURE_VALID_MAX`) |

```text
< -35 °C        invalid
-35 ... <20 °C  valid, low temperature
20 ... 26 °C    normal range
>26 ... 75 °C   valid, high temperature
>75 °C          invalid
```

The alarm thresholds can be changed at compile time using:

- `DHT_TEMPERATURE_ALARM_MIN_CONFIG`
- `DHT_TEMPERATURE_ALARM_MAX_CONFIG`

The validation limits are compile-time constants:

- `DHT_TEMPERATURE_VALID_MIN`
- `DHT_TEMPERATURE_VALID_MAX`

A failed DHT22 read is reported as a device error. Values outside the validation range are reported as invalid data as well.

### Humidity

| Parameter | Value |
|---|---:|
| Valid minimum | 10 % (`DHT_HUMIDITY_VALID_MIN`) |
| Alarm minimum | 20 % (`DHT_HUMIDITY_ALARM_MIN_CONFIG`) |
| Alarm maximum | 80 % (`DHT_HUMIDITY_ALARM_MAX_CONFIG`) |
| Valid maximum | 90 % (`DHT_HUMIDITY_VALID_MAX`) |

```text
<10 %          invalid
10 ... <20 %   valid, low humidity
20 ... 80 %    normal range
>80 ... 90 %   valid, high humidity
>90 %          invalid
```

The alarm thresholds can be changed at compile time using:

- `DHT_HUMIDITY_ALARM_MIN_CONFIG`
- `DHT_HUMIDITY_ALARM_MAX_CONFIG`

The validation limits are compile-time constants:

- `DHT_HUMIDITY_VALID_MIN`
- `DHT_HUMIDITY_VALID_MAX`

### Light

The LDR produces an ADC value which is converted to lux using the configured LDR model.

#### ADC validation

| Parameter | Value |
|---|---:|
| Valid minimum | 50 (`LDR_ADC_VALID_MIN`) |
| Valid maximum | 4045 (`LDR_ADC_VALID_MAX`) |

The current implementation treats the boundary values themselves as outside the working ADC range. This allows ADC values close to the supply rails to be reported as device errors.

#### Lux

| Parameter | Value |
|---|---:|
| Valid minimum | 1 lux (`LDR_LUX_VALID_MIN`) |
| Alarm minimum | 10 lux (`LDR_LUX_ALARM_MIN_CONFIG`) |
| Low-light threshold | 600 lux (`LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG`) |
| Alarm maximum | 10,000 lux (`LDR_LUX_ALARM_MAX_CONFIG`) |
| Valid maximum | 70,000 lux (`LDR_LUX_VALID_MAX`) |

```text
<1 lux               invalid
1 ... <10 lux        valid, minimum-light alarm
10 ... <600 lux      valid, low-light condition
600 ... 10,000 lux   normal operating range
>10,000 ... 70,000   valid, maximum-light alarm
>70,000 lux          invalid
```

The following light thresholds can be changed at compile time:

- `LDR_LUX_ALARM_MIN_CONFIG`
- `LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG`
- `LDR_LUX_ALARM_MAX_CONFIG`

The ADC and lux validation limits are also compile-time constants:

- `LDR_ADC_VALID_MIN`
- `LDR_ADC_VALID_MAX`
- `LDR_LUX_VALID_MIN`
- `LDR_LUX_VALID_MAX`

The LDR conversion model uses additional compile-time constants:

- `LDR_GAMMA`
- `RL10`
- `LDR_R_DIV_OHM`
- `LDR_VCC_V`

## Status Registers

Sensor and system states are represented by bit flags stored in 16-bit status registers.

The DHT and LDR status registers can represent device errors, invalid data, stale-data flags, and sensor alarm conditions.

The system status register can represent command activity, silent mode, sensor errors, and WiFi/MQTT communication errors.

This bit-field approach allows several independent conditions to be represented simultaneously in one status value.

The telemetry status contains the following system-level conditions:

- `STATUS_DEVICE_SILENT_MODE`
- `STATUS_LDR_ERR`
- `STATUS_DHT_ERR`
- `STATUS_MQTT_ERR`
- `STATUS_WIFI_ERR`

## Buttons and Indication

Four button inputs are defined. The current controls provide:

- manual command generation;
- silent mode;
- WiFi disconnection/recovery testing.

Button events are stored in a 16-bit button-state register. A manual button press creates a `manual_read` command and places it into a command queue with a sequence number. The queue can contain up to four pending commands.

LEDs indicate:

- manual/command activity;
- automatic light operation and light alarms;
- temperature alarms;
- humidity alarms;
- silent mode.

## Communication

The device connects through WiFi and publishes data through MQTT.

The MQTT interface uses the following topics:

- `iot-course/OleksiiPok/sensors`
- `iot-course/OleksiiPok/status`
- `iot-course/OleksiiPok/commands`

Sensor data and status are published as separate JSON messages. Command messages contain a command name and sequence number.

In silent mode, sensor-data publication is suppressed while the status message continues to be published. This makes the silent state visible without transmitting the sensor-data message.

MQTT connection recovery is performed automatically. A connection cycle allows up to five attempts with a five-second interval; after that, the next cycle is delayed by five minutes.

The project also contains HTTP POST support for complete telemetry serialization. The HTTP sending path is available as a module but is not part of the active main loop in the current firmware configuration.

## Serialization

The project provides separate serializers for:

- complete telemetry;
- sensor data;
- status data;
- commands.

The primary telemetry serializer uses `snprintf`. An alternative ArduinoJson serializer can be selected at compile time with:

`TELEMETRY_SERIALIZER_ARDUINO_JSON`

A telemetry deserializer is also provided for validating and reconstructing a `Telemetry` structure from JSON. It checks the protocol version and required field types before accepting the data.

## Configuration

Sensor thresholds, sensor validation limits, sensor conversion parameters, communication intervals, and hardware pins are compile-time configurable.

The main configuration is located in `src/config.h`. MQTT settings are located in `src/mqtt/mqtt_config.h`.

Current values are primarily intended for testing and can be adjusted for actual deployment.
