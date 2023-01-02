#pragma once

#include "globals.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Export functions
void initSensors();
void readSensors();