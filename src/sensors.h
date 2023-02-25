#pragma once

#include "globals.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_TSL2561_U.h>

// Export functions
void initSensors();
void readSensors();

// CPU temperature
extern "C" uint8_t temprature_sens_read();