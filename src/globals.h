#pragma once

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>
#include <SPIFFS.h>

#if DEBUG
#include <StreamDebugger.h>
#endif

// Set serial for debug console (to the Serial Monitor)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Extern TinyGsm and PubSubClient
extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient mqtt;

// Export functions
void rebootDevice();
void packageAndSendMQTT(String value, String topic);
const char* stringToChar(String value);
int stringToInt(String value);
void setCPUFrequency(int frequency);

// Extern configuration variables
extern String apn, apnusername, apnpassword, simpin, timezone, dst, apssid, appassword, appowersaving, mqttaddress, mqttport, mqttclientname, mqttusername, mqttpassword, locationgnssmode, locationdpo, sensorsenable, sensorsenablebme280, sensorbme280i2caddress, mqttsensorsbme280temperature, mqttsensorsbme280pressure, mqttsensorsbme280humidity, sensorsenabletsl2561, sensortsl2561i2caddress, sensortsl2561gain, mqttsensorstsl2561lux, sensorsenablepir, sensorpirpin, mqttsensorspir, sensorsenablecputemp, mqttsensorscputemp, sensorsenablecpufreq, mqttsensorscpufreq, sensorsenablefreeram, mqttsensorsfreeram, sensorsenablehall, mqttsensorshall, mqttmodeminfo, mqttmodemccid, mqttmodemimsi, mqttmodemoperator, mqttmodemsignalquality, mqttmodempublicip, mqttbatterypercentage, mqttbatteryvoltage, mqttbatterystatus, mqttlocationtype, mqttlocationlatitude, mqttlocationlongitude, mqttlocationspeed, mqttlocationaltitude, mqttlocationaccuracy, ftpenable, ftpuser, ftppass, bluetoothibeaconenable, bluetoothibeaconuuid, mqttuptime, mqttversion, dynamicfrequency, lowpowermodeonbattery;
extern int connectionRetries;