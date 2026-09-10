# IoT Project

[🇬🇧 English](./README/README.en.md) | [🇺🇦 Українська](./README/README.uk.md)

<img src="./images/wokwi.png" alt="Project Circuit" width="700">

## Project Description

The project is an ESP32 firmware organized into independent functional modules.

The main modules are responsible for:
- sensor data acquisition;
- telemetry and status processing;
- button handling;
- LED indication;
- WiFi and MQTT communication;
- Serial Monitor output.

The main measurement flow is:

```text
DHT22 ──> Temperature ──┐
        Humidity ───────┤
                        ├──> Validation ──> Status
LDR ──> ADC ──> Lux ────┘                    │
                                             ├──> LED indication
                                             ├──> MQTT
                                             └──> Serial Monitor
```

Each sensor value is processed through validation limits and operating/alarm thresholds.

## Data Structure

The internal telemetry contains device information, time data, sensor measurements, and status registers.

```cpp
struct DHTData {
    float temperature; // °C
    float humidity;    // %
    uint8_t status;
};

struct LDRData {
    uint16_t raw; // ADC data
    float lux;    // lux
    uint8_t status;
};

struct Telemetry {
    uint64_t deviceId;
    uint32_t timestamp;
    uint32_t uptime;
    uint32_t sequence;
    DHTData dht;
    LDRData ldr;
    uint8_t status; // system status register
};
```

MQTT data is published as separate sensor-data and status messages.

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

The validation limits are also compile-time constants:
- `DHT_TEMPERATURE_VALID_MIN`
- `DHT_TEMPERATURE_VALID_MAX`

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

The validation limits are also compile-time constants:
- `DHT_HUMIDITY_VALID_MIN`
- `DHT_HUMIDITY_VALID_MAX`

### Light

The LDR produces an ADC value which is converted to lux.

#### ADC validation

| Parameter | Value |
|---|---:|
| Valid minimum | 50 (`LDR_ADC_VALID_MIN`) |
| Valid maximum | 4045 (`LDR_ADC_VALID_MAX`) |

The current implementation treats the boundary values themselves as outside the working ADC range.

#### Lux

| Parameter | Value |
|---|---:|
| Valid minimum | 1 lux (`LDR_LUX_VALID_MIN`) |
| Alarm minimum | 10 lux (`LDR_LUX_ALARM_MIN_CONFIG`) |
| Low-light threshold | 600 lux (`LDR_LUX_THRESHOLD_LIGHT_LOW_CONFIG`) |
| Alarm maximum | 10,000 lux (`LDR_LUX_ALARM_MAX_CONFIG`) |
| Valid maximum | 70,000 lux (`LDR_LUX_VALID_MAX`) |

```text
<1 lux             invalid
1 ... <10 lux      valid, minimum-light alarm
10 ... <600 lux    valid, low-light condition
600 ... 10,000 lux normal operating range
>10,000 ... 70,000 valid, maximum-light alarm
>70,000 lux        invalid
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

The LDR conversion model uses additional compile-time constants such as `LDR_GAMMA`, `RL10`, `LDR_R_DIV_OHM`, and `LDR_VCC_V`.

## Status Registers

Sensor and system states are represented by bit flags.

The DHT, LDR, and system status registers can indicate device errors, invalid data, stale data, alarm conditions, and system communication errors.

This allows several independent conditions to be represented simultaneously in one status value.

## Buttons and Indication

Three buttons provide control of:
- manual light mode;
- silent mode;
- WiFi disconnection/recovery testing.

LEDs indicate light modes, light alarms, temperature alarms, humidity alarms, and silent mode.

## Communication

The device connects through WiFi and publishes telemetry through MQTT.

The project also supports Serial Monitor output.

WiFi disconnection starts the automatic connection recovery process.

## Configuration

Sensor thresholds, sensor validation limits, sensor conversion parameters, and operating intervals are compile-time configurable.

Current values are primarily intended for testing and can be adjusted for actual deployment.

[🇬🇧 English](./README/README.en.md) | [🇺🇦 Українська](./README/README.uk.md)
