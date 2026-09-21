#pragma once

// Скопіюй цей файл у secrets.h і заповни своїми значеннями.
// secrets.h у .gitignore — НІКОЛИ не комітити приватний ключ у git!

// ═══════════════════════════════════════════════════════════
// WI-FI
// ═══════════════════════════════════════════════════════════
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// ═══════════════════════════════════════════════════════════
// AWS IOT CORE
// ═══════════════════════════════════════════════════════════
// Client ID МАЄ дорівнювати імені Thing — інакше Policy заблокує (слайд 16)
#define THINGNAME "esp32lecture10"
// Твій endpoint: AWS IoT Console → Settings → Device data endpoint
#define AWS_IOT_ENDPOINT "xxxxxxxxxxxxx-ats.iot.eu-north-1.amazonaws.com"

// ═══════════════════════════════════════════════════════════
// СЕРТИФІКАТИ — 3 файли зі слайда 10
// Вставити вміст .pem файлів, завантажених у Занятті 9, як є
// ═══════════════════════════════════════════════════════════

// AmazonRootCA1.pem — перевірка сервера ("це справді AWS?")
static const char AWS_CERT_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----
...вставити вміст AmazonRootCA1.pem...
-----END CERTIFICATE-----
)EOF";

// certificate.pem.crt — паспорт пристрою ("ось хто я")
static const char AWS_CERT_CRT[] = R"EOF(
MIIDWTCCAkGgAwIBAgIUfXUWZ8zZ8KVNJHZn3tMcyjHD+pMwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI2MDkyMTExMjYw
N1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK0lHBVMqS+maq4jCbO+
xzFxHRQph24QejEw8utdi7grpxeu2nItwgqEksoeGW1bjqSGEuqiBzut6aqMI/kp
f8qYmVNd4MuJ22lvgy4G1yF2H8qp59ZXxafqBxSoAvLBwKkcensfhwWrD+i5X2Si
k6e+WLcEBcinkph/0YWt/vtebgffeWKxg9p2ok1fpR5FH0f+RppZjOCACBm3Yvki
T6gRvZ/UFBzrcBcBDZCd6DIN14Hrysjwds8hjACi3dmpozPOUibrO6vIYkirBL8H
uCu6gzUI/EH+SIpV4+H/Adu/vyAOBE3xuI2lIT3bvYXXImoHTDCwK2EHyzxp8+e3
jkkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUZzA0B+8DBP8aISOuYlwkVXAdHF8wHQYD
VR0OBBYEFCI2ZfXBjcjQJ3Yg8GID/sHhFmGTMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBaQg06VKUYuEPn034Byt34Oys/
ZfhUjx3xzqXnSfAbik2lZVnH6hm+nTfTy6rIvwVd34tkm0FGIRcCvDYp/mVV0Deg
LMYVEziN9C8U9M0Hj7Wj2QxDugxcIaT3tX62JP9TqOdTXd0oXdp3g1mAzDTioY3I
0PGtsklcN5GTfXy4DrJY3q9Q6T1GL+ItU5lALX/EMnSXQG+gEM32Y9iX/u5Jvwo5
GdfEmcYnMORaGY9PsKDLwxWOIxVswlOucsaBql7hPOf9JDt0gVN2AnKE9EUhZFa+
OaSuaewfhcAfLItW5D0pXwDsNKAX6rgWpcuFguonIVLvnA9fUzc64AmKnvlb
)EOF";

// private.pem.key — секретний доказ ("паспорт справді мій")
// НІКОЛИ не комітити цей файл у git!
static const char AWS_CERT_PRIVATE[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
...вставити вміст private.pem.key...
-----END RSA PRIVATE KEY-----
)EOF";
