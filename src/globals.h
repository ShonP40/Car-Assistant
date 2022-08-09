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