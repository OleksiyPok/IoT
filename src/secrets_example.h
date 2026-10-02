// src/secrets_example.h

#pragma once

// ═══════════════════════════════════════════════════════════
// WI-FI
// ═══════════════════════════════════════════════════════════
#define WIFI_SSID "SSID" // SSID
#define WIFI_PASSWORD "" // Password (empty for Wokwi-GUEST)

// ═══════════════════════════════════════════════════════════
// MQTT test server
// ═══════════════════════════════════════════════════════════
#define MQTT_TEST_BROKER "broker.hivemq.com"
#define MQTT_TEST_PORT 1883
// #define MQTT_TEST_CLIENT_ID "xxxx"

// ═══════════════════════════════════════════════════════════
// AWS IOT CORE
// ═══════════════════════════════════════════════════════════
#define MQTT_AWS_PORT 8883
// THINGNAME == MQTT client ID
// #define AWS_THINGNAME "xxxx"
// Endpoint: AWS IoT Console → Settings → Device data endpoint
#define AWS_IOT_ENDPOINT "xxxx.amazonaws.com"

// ═══════════════════════════════════════════════════════════
// CERTIFICATES
// ═══════════════════════════════════════════════════════════

// AmazonRootCA1.pem
static const char AWS_CERT_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----
...вставити вміст AmazonRootCA1.pem...
-----END CERTIFICATE-----
)EOF";

// certificate.pem.crt
static const char AWS_CERT_CRT[] = R"EOF(
-----BEGIN CERTIFICATE-----
...вставити вміст certificate.pem.crt...
-----END CERTIFICATE-----
)EOF";

// private.pem.key
static const char AWS_CERT_PRIVATE[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
...вставити вміст private.pem.key...
-----END RSA PRIVATE KEY-----
)EOF";
