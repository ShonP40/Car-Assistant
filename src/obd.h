#pragma once

#include "globals.h"

#include <BluetoothSerial.h>
#include <ELMduino.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
#include <esp_err.h>

// Export functions
void initOBD();
void readOBD();