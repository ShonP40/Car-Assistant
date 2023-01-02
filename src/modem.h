#pragma once

#include "globals.h"

// Export functions
void initModem();
void initNetwork();
void getNetworkTime();
void batteryInfo();
void getNetInfo();

// Extern the initialized variable
extern bool initialized;