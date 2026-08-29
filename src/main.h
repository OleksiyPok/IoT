// src/main.h

#pragma once

#define STATUS_OK 0b00000000
#define STATUS_LDR_ERR 0b00000001  // bit 0: LDR error
#define STATUS_DHT_ERR 0b00000100  // bit 2: DHT22 error
#define STATUS_MQTT_ERR 0b01000000 // bit 6: MQTT error
#define STATUS_WIFI_ERR 0b10000000 // bit 7: Wi-Fi error