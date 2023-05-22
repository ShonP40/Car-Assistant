#pragma once

#include "globals.h"

// Export functions
void initModem();
void initNetwork();
void getNetworkTime();
void batteryInfo();
void getNetInfo();
void getLocationInfo();

// Extern variables
extern bool initialized;
extern bool charging;
extern bool stationary;