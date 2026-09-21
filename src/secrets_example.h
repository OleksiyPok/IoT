// src/secrets_example.h

#pragma once

// ═══════════════════════════════════════════════════════════
// WI-FI
// ═══════════════════════════════════════════════════════════
#define WIFI_SSID "Wokwi-GUEST" // SSID
#define WIFI_PASSWORD ""        // Password (empty for Wokwi-GUEST)

// ═══════════════════════════════════════════════════════════
// MQTT broker
// ═══════════════════════════════════════════════════════════
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "...."

// ═══════════════════════════════════════════════════════════
// AWS IOT CORE
// ═══════════════════════════════════════════════════════════
#define MQTT_AWS_PORT 8883
// THINGNAME == MQTT client ID
#define THINGNAME "OleksiiPok-esp32-a-wokwi"
// Endpoint: AWS IoT Console → Settings → Device data endpoint
#define AWS_IOT_ENDPOINT "a14905hl20lubp-ats.iot.us-east-1.amazonaws.com"

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
