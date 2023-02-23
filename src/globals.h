#pragma once

#include "config.h"

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>
#include <SPIFFS.h>

#if DEBUG
#include <StreamDebugger.h>
#endif

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Extern TinyGsm and PubSubClient
extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient mqtt;

// Export functions
void packageAndSendMQTT(String value, String topic);