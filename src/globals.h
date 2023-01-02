#pragma once

#include "config.h"

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#if DEBUG
#include <StreamDebugger.h>
#endif

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Extern modem and mqtt
extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient mqtt;

// Export packageAndSendMQTT
void packageAndSendMQTT(String value, String topic);