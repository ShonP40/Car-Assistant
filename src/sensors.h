#pragma once

#include "globals.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>

#if BME280_ENABLED
#include <Adafruit_BME280.h>
#endif

#if TSL2561_ENABLED
#include <Adafruit_TSL2561_U.h>
#endif

// Export functions
void initSensors();
void readSensors();