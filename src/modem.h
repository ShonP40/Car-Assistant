#pragma once

#include "globals.h"

// Export functions
void initModem();
void initNetwork();
void getNetworkTime();
void batteryInfo();
void getNetInfo();

// Extern variables
extern bool initialized;