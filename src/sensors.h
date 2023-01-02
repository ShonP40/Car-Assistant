#pragma once

#include "globals.h"

#if SENSORS_ENABLED

#if BME280_ENABLED || TSL2561_ENABLED
#include <Wire.h>
#include <Adafruit_Sensor.h>
#endif

#if BME280_ENABLED
#include <Adafruit_BME280.h>
#endif

#if TSL2561_ENABLED
#include <Adafruit_TSL2561_U.h>
#endif

// Export functions
void initSensors();
void readSensors();

#endif