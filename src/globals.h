#pragma once

#include "config.h"

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>
#include <SPIFFS.h>

#if DEBUG
#include <StreamDebugger.h>
#endif

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Extern TinyGsm and PubSubClient
extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient mqtt;

// Export functions
void packageAndSendMQTT(String value, String topic);
const char* stringToChar(String value);
int stringToInt(String value);

// Extern configuration variables
extern String apn, apnusername, apnpassword, simpin, apssid, appassword, mqttaddress, mqttport, mqttclientname, mqttusername, mqttpassword, locationgnssmode, locationdpo, sensorsenable, sensorsenablebme280, sensorbme280i2caddress, mqttsensorsbme280temperature, mqttsensorsbme280pressure, mqttsensorsbme280humidity, sensorsenabletsl2561, sensortsl2561i2caddress, sensortsl2561gain, mqttsensorstsl2561lux, sensorsenablepir, sensorpirpin, mqttsensorspir, mqttmodeminfo, mqttmodemccid, mqttmodemimsi, mqttmodemoperator, mqttmodemsignalquality, mqttmodempublicip, mqttbatterypercentage, mqttbatteryvoltage, mqttbatterystatus, mqttuptime, mqttlocationtype, mqttlocationlatitude, mqttlocationlongitude, mqttlocationspeed, mqttlocationaltitude, mqttlocationaccuracy;