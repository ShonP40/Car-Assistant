#include "globals.h"
#include "modem.h"
#include "sensors.h"
#include "html.h"
#include "ftp.h"
#include "obd.h"

// WiFi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

// Configuration variables
String apn, apnusername, apnpassword, simpin, apssid, appassword, mqttaddress, mqttport, mqttclientname, mqttusername, mqttpassword, locationgnssmode, locationdpo, sensorsenable, sensorsenablebme280, sensorbme280i2caddress, mqttsensorsbme280temperature, mqttsensorsbme280pressure, mqttsensorsbme280humidity, sensorsenabletsl2561, sensortsl2561i2caddress, sensortsl2561gain, mqttsensorstsl2561lux, sensorsenablepir, sensorpirpin, mqttsensorspir, sensorsenablecputemp, mqttsensorscputemp, sensorsenablecpufreq, mqttsensorscpufreq, sensorsenablefreeram, mqttsensorsfreeram, sensorsenablehall, mqttsensorshall, mqttmodeminfo, mqttmodemccid, mqttmodemimsi, mqttmodemoperator, mqttmodemsignalquality, mqttmodempublicip, mqttbatterypercentage, mqttbatteryvoltage, mqttbatterystatus, mqttlocationtype, mqttlocationlatitude, mqttlocationlongitude, mqttlocationspeed, mqttlocationaltitude, mqttlocationaccuracy, ftpenable, ftpuser, ftppass, bluetoothenable, bluetoothname, bluetoothobdidentifier, bluetoothobdmqttobdpayload, bluetoothobdmqttobddebug, bluetoothobdmqttengineload, bluetoothobdmqttenginecoolanttemp, bluetoothobdmqttshorttermfueltrimbank1, bluetoothobdmqttlongtermfueltrimbank1, bluetoothobdmqttshorttermfueltrimbank2, bluetoothobdmqttlongtermfueltrimbank2, bluetoothobdmqttfuelpressure, bluetoothobdmqttmanifoldpressure, bluetoothobdmqttrpm, bluetoothobdmqttkph, bluetoothobdmqttmph, bluetoothobdmqtttimingadvance, bluetoothobdmqttintakeairtemp, bluetoothobdmqttmafrate, bluetoothobdmqttthrottle, bluetoothobdmqttauxinputstatus, bluetoothobdmqttruntime, bluetoothobdmqttdisttravelwithmil, bluetoothobdmqttfuelrailpressure, bluetoothobdmqttfuelrailguagepressure, bluetoothobdmqttcommandedegr, bluetoothobdmqttegrerror, bluetoothobdmqttcommandedevappurge, bluetoothobdmqttfuellevel, bluetoothobdmqttwarmupssincecodescleared, bluetoothobdmqttdistsincecodescleared, bluetoothobdmqttevapsysvappressure, bluetoothobdmqttabsbaropressure, bluetoothobdmqttcattempb1s1, bluetoothobdmqttcattempb2s1, bluetoothobdmqttcattempb1s2, bluetoothobdmqttcattempb2s2, bluetoothobdmqttctrlmodvoltage, bluetoothobdmqttabsload, bluetoothobdmqttcommandedairfuelratio, bluetoothobdmqttrelativethrottle, bluetoothobdmqttambientairtemp, bluetoothobdmqttabsthrottleposb, bluetoothobdmqttabsthrottleposc, bluetoothobdmqttabsthrottleposd, bluetoothobdmqttabsthrottlepose, bluetoothobdmqttabsthrottleposf, bluetoothobdmqttcommandedthrottleactuator, bluetoothobdmqtttimerunwithmil, bluetoothobdmqtttimesincecodescleared, bluetoothobdmqttmaxmafrate, bluetoothobdmqttethonolpercent, bluetoothobdmqttabsevapsysvappressure, bluetoothobdmqttevapsysvappressure2, bluetoothobdmqttabsfuelrailpressure, bluetoothobdmqttrelativepedalpos, bluetoothobdmqtthybridbatlife, bluetoothobdmqttoiltemp, bluetoothobdmqttfuelinjecttiming, bluetoothobdmqttfuelrate, bluetoothobdmqttdemandedtorque, bluetoothobdmqtttorque, bluetoothobdmqttreferencetorque, bluetoothobdmqttbatteryvoltage, bluetoothobdmqttvin, mqttuptime, mqttversion, dynamicfrequency, lowpowermodeonbattery;

// Search for parameter in HTTP POST request
const String configIDs[] = {"apn", "apnusername", "apnpassword", "simpin", "apssid", "appassword", "mqttaddress", "mqttport", "mqttclientname", "mqttusername", "mqttpassword", "locationgnssmode", "locationdpo", "sensorsenable", "sensorsenablebme280", "sensorbme280i2caddress", "mqttsensorsbme280temperature", "mqttsensorsbme280pressure", "mqttsensorsbme280humidity", "sensorsenabletsl2561", "sensortsl2561i2caddress", "sensortsl2561gain", "mqttsensorstsl2561lux", "sensorsenablepir", "sensorpirpin", "mqttsensorspir", "sensorsenablecputemp", "mqttsensorscputemp", "sensorsenablecpufreq", "mqttsensorscpufreq", "sensorsenablefreeram", "mqttsensorsfreeram", "sensorsenablehall", "mqttsensorshall", "mqttmodeminfo", "mqttmodemccid", "mqttmodemimsi", "mqttmodemoperator", "mqttmodemsignalquality", "mqttmodempublicip", "mqttbatterypercentage", "mqttbatteryvoltage", "mqttbatterystatus", "mqttlocationtype", "mqttlocationlatitude", "mqttlocationlongitude", "mqttlocationspeed", "mqttlocationaltitude", "mqttlocationaccuracy", "ftpenable", "ftpuser", "ftppass", "bluetoothenable", "bluetoothname", "bluetoothobdidentifier", "bluetoothobdmqttobdpayload", "bluetoothobdmqttobddebug", "bluetoothobdmqttengineload", "bluetoothobdmqttenginecoolanttemp", "bluetoothobdmqttshorttermfueltrimbank1", "bluetoothobdmqttlongtermfueltrimbank1", "bluetoothobdmqttshorttermfueltrimbank2", "bluetoothobdmqttlongtermfueltrimbank2", "bluetoothobdmqttfuelpressure", "bluetoothobdmqttmanifoldpressure", "bluetoothobdmqttrpm", "bluetoothobdmqttkph", "bluetoothobdmqttmph", "bluetoothobdmqtttimingadvance", "bluetoothobdmqttintakeairtemp", "bluetoothobdmqttmafrate", "bluetoothobdmqttthrottle", "bluetoothobdmqttauxinputstatus", "bluetoothobdmqttruntime", "bluetoothobdmqttdisttravelwithmil", "bluetoothobdmqttfuelrailpressure", "bluetoothobdmqttfuelrailguagepressure", "bluetoothobdmqttcommandedegr", "bluetoothobdmqttegrerror", "bluetoothobdmqttcommandedevappurge", "bluetoothobdmqttfuellevel", "bluetoothobdmqttwarmupssincecodescleared", "bluetoothobdmqttdistsincecodescleared", "bluetoothobdmqttevapsysvappressure", "bluetoothobdmqttabsbaropressure", "bluetoothobdmqttcattempb1s1", "bluetoothobdmqttcattempb2s1", "bluetoothobdmqttcattempb1s2", "bluetoothobdmqttcattempb2s2", "bluetoothobdmqttctrlmodvoltage", "bluetoothobdmqttabsload", "bluetoothobdmqttcommandedairfuelratio", "bluetoothobdmqttrelativethrottle", "bluetoothobdmqttambientairtemp", "bluetoothobdmqttabsthrottleposb", "bluetoothobdmqttabsthrottleposc", "bluetoothobdmqttabsthrottleposd", "bluetoothobdmqttabsthrottlepose", "bluetoothobdmqttabsthrottleposf", "bluetoothobdmqttcommandedthrottleactuator", "bluetoothobdmqtttimerunwithmil", "bluetoothobdmqtttimesincecodescleared", "bluetoothobdmqttmaxmafrate", "bluetoothobdmqttethonolpercent", "bluetoothobdmqttabsevapsysvappressure", "bluetoothobdmqttevapsysvappressure2", "bluetoothobdmqttabsfuelrailpressure", "bluetoothobdmqttrelativepedalpos", "bluetoothobdmqtthybridbatlife", "bluetoothobdmqttoiltemp", "bluetoothobdmqttfuelinjecttiming", "bluetoothobdmqttfuelrate", "bluetoothobdmqttdemandedtorque", "bluetoothobdmqtttorque", "bluetoothobdmqttreferencetorque", "bluetoothobdmqttbatteryvoltage", "bluetoothobdmqttvin", "mqttuptime", "mqttversion", "dynamicfrequency", "lowpowermodeonbattery"};

// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path) {
  #if DEBUG
  SerialMon.printf("Reading file: %s\r\n", path);
  #endif

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    #if DEBUG
    SerialMon.println("Failed to open file for reading");
    #endif
    return String();
  }
  
  String fileContent;
  while (file.available()) {
    fileContent = file.readStringUntil('\n');
    break;
  }

  file.close();

  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char * path, const char * message) {
  fs.remove(path);
  #if DEBUG
  SerialMon.printf("Deleting existing file: %s\r\n", path);

  SerialMon.printf("Writing file: %s\r\n", path);
  #endif

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    #if DEBUG
    SerialMon.println("Failed to open file for writing");
    #endif
    return;
  }
  if (file.print(message)) {
    #if DEBUG
    SerialMon.println("File written");
  } else {
    SerialMon.println("Write failed");
    #endif
  }

  file.close();
}

const char* stringToChar(String value) {
  return value.c_str();
}

int stringToInt(String value) {
  return value.toInt();
}

void loadConfig() {
  // Open file for reading
  String configFile = readFile(SPIFFS, "/config.json");
  // Allocate a temporary JsonDocument
  DynamicJsonDocument doc(8192);
  // Deserialize the JSON document
  deserializeJson(doc, configFile);
  apn = doc["apn"] | "yourapn.com";
  apnusername = doc["apnusername"] | "user";
  apnpassword = doc["apnpassword"] | "pass";
  simpin = doc["simpin"] | "";
  apssid = doc["apssid"] | "Car Assistant";
  appassword = doc["appassword"] | "12345678";
  mqttaddress = doc["mqttaddress"] | "yourbroker.com";
  mqttport = doc["mqttport"] | "1883";
  mqttclientname = doc["mqttclientname"] | "car-assistant";
  mqttusername = doc["mqttusername"] | "user";
  mqttpassword = doc["mqttpassword"] | "pass";
  locationgnssmode = doc["locationgnssmode"] | "0";
  locationdpo = doc["locationdpo"] | "1";
  sensorsenable = doc["sensorsenable"] | "true";
  sensorsenablebme280 = doc["sensorsenablebme280"] | "true";
  sensorbme280i2caddress = doc["sensorbme280i2caddress"] | "0x76";
  mqttsensorsbme280temperature = doc["mqttsensorsbme280temperature"] | "temperature";
  mqttsensorsbme280pressure = doc["mqttsensorsbme280pressure"] | "pressure";
  mqttsensorsbme280humidity = doc["mqttsensorsbme280humidity"] | "humidity";
  sensorsenabletsl2561 = doc["sensorsenabletsl2561"] | "true";
  sensortsl2561i2caddress = doc["sensortsl2561i2caddress"] | "0x39";
  sensortsl2561gain = doc["sensortsl2561gain"] | "auto";
  mqttsensorstsl2561lux = doc["mqttsensorstsl2561lux"] | "lux";
  sensorsenablepir = doc["sensorsenablepir"] | "true";
  sensorpirpin = doc["sensorpirpin"] | "19";
  mqttsensorspir = doc["mqttsensorspir"] | "pir";
  sensorsenablecputemp = doc["sensorsenablecputemp"] | "true";
  mqttsensorscputemp = doc["mqttsensorscputemp"] | "cpu-temperature";
  sensorsenablecpufreq = doc["sensorsenablecpufreq"] | "true";
  mqttsensorscpufreq = doc["mqttsensorscpufreq"] | "cpu-frequency";
  sensorsenablefreeram = doc["sensorsenablefreeram"] | "true";
  mqttsensorsfreeram = doc["mqttsensorsfreeram"] | "free-ram";
  sensorsenablehall = doc["sensorsenablehall"] | "true";
  mqttsensorshall = doc["mqttsensorshall"] | "hall";
  mqttmodeminfo = doc["mqttmodeminfo"] | "modem-info";
  mqttmodemccid = doc["mqttmodemccid"] | "ccid";
  mqttmodemimsi = doc["mqttmodemimsi"] | "imsi";
  mqttmodemoperator = doc["mqttmodemoperator"] | "operator";
  mqttmodemsignalquality = doc["mqttmodemsignalquality"] | "signal-quality";
  mqttmodempublicip = doc["mqttmodempublicip"] | "public-ip";
  mqttbatterypercentage = doc["mqttbatterypercentage"] | "battery-percentage";
  mqttbatteryvoltage = doc["mqttbatteryvoltage"] | "battery-voltage";
  mqttbatterystatus = doc["mqttbatterystatus"] | "battery-status";
  mqttlocationtype = doc["mqttlocationtype"] | "location-type";
  mqttlocationlatitude = doc["mqttlocationlatitude"] | "latitude";
  mqttlocationlongitude = doc["mqttlocationlongitude"] | "longitude";
  mqttlocationspeed = doc["mqttlocationspeed"] | "speed";
  mqttlocationaltitude = doc["mqttlocationaltitude"] | "altitude";
  mqttlocationaccuracy = doc["mqttlocationaccuracy"] | "location-accuracy";
  ftpenable = doc["ftpenable"] | "false";
  ftpuser = doc["ftpuser"] | "carassistant";
  ftppass = doc["ftppass"] | "12345678";
  bluetoothenable = doc["bluetoothenable"] | "true";
  bluetoothname = doc["bluetoothname"] | "Car Assistant";
  bluetoothobdidentifier = doc["bluetoothobdidentifier"] | "Android-Vlink";
  bluetoothobdmqttobdpayload = doc["bluetoothobdmqttobdpayload"] | "obd-payload";
  bluetoothobdmqttobddebug = doc["bluetoothobdmqttobddebug"] | "obd-debug";
  bluetoothobdmqttengineload = doc["bluetoothobdmqttengineload"] | "obd-engine-load";
  bluetoothobdmqttenginecoolanttemp = doc["bluetoothobdmqttenginecoolanttemp"] | "obd-engine-coolant-temp";
  bluetoothobdmqttshorttermfueltrimbank1 = doc["bluetoothobdmqttshorttermfueltrimbank1"] | "obd-short-term-fuel-trim-bank-1";
  bluetoothobdmqttlongtermfueltrimbank1 = doc["bluetoothobdmqttlongtermfueltrimbank1"] | "obd-long-term-fuel-trim-bank-1";
  bluetoothobdmqttshorttermfueltrimbank2 = doc["bluetoothobdmqttshorttermfueltrimbank2"] | "obd-short-term-fuel-trim-bank-2";
  bluetoothobdmqttlongtermfueltrimbank2 = doc["bluetoothobdmqttlongtermfueltrimbank2"] | "obd-long-term-fuel-trim-bank-2";
  bluetoothobdmqttfuelpressure = doc["bluetoothobdmqttfuelpressure"] | "obd-fuel-pressure";
  bluetoothobdmqttmanifoldpressure = doc["bluetoothobdmqttmanifoldpressure"] | "obd-manifold-pressure";
  bluetoothobdmqttrpm = doc["bluetoothobdmqttrpm"] | "obd-rpm";
  bluetoothobdmqttkph = doc["bluetoothobdmqttkph"] | "obd-kph";
  bluetoothobdmqttmph = doc["bluetoothobdmqttmph"] | "obd-mph";
  bluetoothobdmqtttimingadvance = doc["bluetoothobdmqtttimingadvance"] | "obd-timing-advance";
  bluetoothobdmqttintakeairtemp = doc["bluetoothobdmqttintakeairtemp"] | "obd-intake-air-temp";
  bluetoothobdmqttmafrate = doc["bluetoothobdmqttmafrate"] | "obd-maf-rate";
  bluetoothobdmqttthrottle = doc["bluetoothobdmqttthrottle"] | "obd-throttle";
  bluetoothobdmqttauxinputstatus = doc["bluetoothobdmqttauxinputstatus"] | "obd-aux-input-status";
  bluetoothobdmqttruntime = doc["bluetoothobdmqttruntime"] | "obd-run-time";
  bluetoothobdmqttdisttravelwithmil = doc["bluetoothobdmqttdisttravelwithmil"] | "obd-dist-travel-with-mil";
  bluetoothobdmqttfuelrailpressure = doc["bluetoothobdmqttfuelrailpressure"] | "obd-fuel-rail-pressure";
  bluetoothobdmqttfuelrailguagepressure = doc["bluetoothobdmqttfuelrailguagepressure"] | "obd-fuel-rail-guage-pressure";
  bluetoothobdmqttcommandedegr = doc["bluetoothobdmqttcommandedegr"] | "obd-commanded-egr";
  bluetoothobdmqttegrerror = doc["bluetoothobdmqttegrerror"] | "obd-egr-error";
  bluetoothobdmqttcommandedevappurge = doc["bluetoothobdmqttcommandedevappurge"] | "obd-commanded-evap-purge";
  bluetoothobdmqttfuellevel = doc["bluetoothobdmqttfuellevel"] | "obd-fuel-level";
  bluetoothobdmqttwarmupssincecodescleared = doc["bluetoothobdmqttwarmupssincecodescleared"] | "obd-warm-ups-since-codes-cleared";
  bluetoothobdmqttdistsincecodescleared = doc["bluetoothobdmqttdistsincecodescleared"] | "obd-dist-since-codes-cleared";
  bluetoothobdmqttevapsysvappressure = doc["bluetoothobdmqttevapsysvappressure"] | "obd-evap-sys-vap-pressure";
  bluetoothobdmqttabsbaropressure = doc["bluetoothobdmqttabsbaropressure"] | "obd-abs-baro-pressure";
  bluetoothobdmqttcattempb1s1 = doc["bluetoothobdmqttcattempb1s1"] | "obd-cat-temp-b1-s1";
  bluetoothobdmqttcattempb2s1 = doc["bluetoothobdmqttcattempb2s1"] | "obd-cat-temp-b2-s1";
  bluetoothobdmqttcattempb1s2 = doc["bluetoothobdmqttcattempb1s2"] | "obd-cat-temp-b1-s2";
  bluetoothobdmqttcattempb2s2 = doc["bluetoothobdmqttcattempb2s2"] | "obd-cat-temp-b2-s2";
  bluetoothobdmqttctrlmodvoltage = doc["bluetoothobdmqttctrlmodvoltage"] | "obd-ctrl-mod-voltage";
  bluetoothobdmqttabsload = doc["bluetoothobdmqttabsload"] | "obd-abs-load";
  bluetoothobdmqttcommandedairfuelratio = doc["bluetoothobdmqttcommandedairfuelratio"] | "obd-commanded-air-fuel-ratio";
  bluetoothobdmqttrelativethrottle = doc["bluetoothobdmqttrelativethrottle"] | "obd-relative-throttle";
  bluetoothobdmqttambientairtemp = doc["bluetoothobdmqttambientairtemp"] | "obd-ambient-air-temp";
  bluetoothobdmqttabsthrottleposb = doc["bluetoothobdmqttabsthrottleposb"] | "obd-abs-throttle-pos-b";
  bluetoothobdmqttabsthrottleposc = doc["bluetoothobdmqttabsthrottleposc"] | "obd-abs-throttle-pos-c";
  bluetoothobdmqttabsthrottleposd = doc["bluetoothobdmqttabsthrottleposd"] | "obd-abs-throttle-pos-d";
  bluetoothobdmqttabsthrottlepose = doc["bluetoothobdmqttabsthrottlepose"] | "obd-abs-throttle-pos-e";
  bluetoothobdmqttabsthrottleposf = doc["bluetoothobdmqttabsthrottleposf"] | "obd-abs-throttle-pos-f";
  bluetoothobdmqttcommandedthrottleactuator = doc["bluetoothobdmqttcommandedthrottleactuator"] | "obd-commanded-throttle-actuator";
  bluetoothobdmqtttimerunwithmil = doc["bluetoothobdmqtttimerunwithmil"] | "obd-time-run-with-mil";
  bluetoothobdmqtttimesincecodescleared = doc["bluetoothobdmqtttimesincecodescleared"] | "obd-time-since-codes-cleared";
  bluetoothobdmqttmaxmafrate = doc["bluetoothobdmqttmaxmafrate"] | "obd-max-maf-rate";
  bluetoothobdmqttethonolpercent = doc["bluetoothobdmqttethonolpercent"] | "obd-ethonol-percent";
  bluetoothobdmqttabsevapsysvappressure = doc["bluetoothobdmqttabsevapsysvappressure"] | "obd-abs-evap-sys-vap-pressure";
  bluetoothobdmqttevapsysvappressure2 = doc["bluetoothobdmqttevapsysvappressure2"] | "obd-evap-sys-vap-pressure-2";
  bluetoothobdmqttabsfuelrailpressure = doc["bluetoothobdmqttabsfuelrailpressure"] | "obd-abs-fuel-rail-pressure";
  bluetoothobdmqttrelativepedalpos = doc["bluetoothobdmqttrelativepedalpos"] | "obd-relative-pedal-pos";
  bluetoothobdmqtthybridbatlife = doc["bluetoothobdmqtthybridbatlife"] | "obd-hybrid-bat-life";
  bluetoothobdmqttoiltemp = doc["bluetoothobdmqttoiltemp"] | "obd-oil-temp";
  bluetoothobdmqttfuelinjecttiming = doc["bluetoothobdmqttfuelinjecttiming"] | "obd-fuel-inject-timing";
  bluetoothobdmqttfuelrate = doc["bluetoothobdmqttfuelrate"] | "obd-fuel-rate";
  bluetoothobdmqttdemandedtorque = doc["bluetoothobdmqttdemandedtorque"] | "obd-demanded-torque";
  bluetoothobdmqtttorque = doc["bluetoothobdmqtttorque"] | "obd-torque";
  bluetoothobdmqttreferencetorque = doc["bluetoothobdmqttreferencetorque"] | "obd-reference-torque";
  bluetoothobdmqttbatteryvoltage = doc["bluetoothobdmqttbatteryvoltage"] | "obd-battery-voltage";
  bluetoothobdmqttvin = doc["bluetoothobdmqttvin"] | "obd-vin";
  mqttuptime = doc["mqttuptime"] | "uptime";
  mqttversion = doc["mqttversion"] | "version";
  dynamicfrequency = doc["dynamicfrequency"] | "false";
  lowpowermodeonbattery = doc["lowpowermodeonbattery"] | "false";

  #if DEBUG
  SerialMon.println("Loaded configuration from config.json");

  // Print all the config values
  SerialMon.println("Configuration:");
  SerialMon.println("  apn: " + apn);
  SerialMon.println("  apnusername: " + apnusername);
  SerialMon.println("  apnpassword: " + apnpassword);
  SerialMon.println("  simpin: " + simpin);
  SerialMon.println("  apssid: " + apssid);
  SerialMon.println("  appassword: " + appassword);
  SerialMon.println("  mqttaddress: " + mqttaddress);
  SerialMon.println("  mqttport: " + mqttport);
  SerialMon.println("  mqttclientname: " + mqttclientname);
  SerialMon.println("  mqttusername: " + mqttusername);
  SerialMon.println("  mqttpassword: " + mqttpassword);
  SerialMon.println("  locationgnssmode: " + locationgnssmode);
  SerialMon.println("  locationdpo: " + locationdpo);
  SerialMon.println("  sensorsenable: " + sensorsenable);
  SerialMon.println("  sensorsenablebme280: " + sensorsenablebme280);
  SerialMon.println("  sensorbme280i2caddress: " + sensorbme280i2caddress);
  SerialMon.println("  mqttsensorsbme280temperature: " + mqttsensorsbme280temperature);
  SerialMon.println("  mqttsensorsbme280pressure: " + mqttsensorsbme280pressure);
  SerialMon.println("  mqttsensorsbme280humidity: " + mqttsensorsbme280humidity);
  SerialMon.println("  sensorsenabletsl2561: " + sensorsenabletsl2561);
  SerialMon.println("  sensortsl2561i2caddress: " + sensortsl2561i2caddress);
  SerialMon.println("  sensortsl2561gain: " + sensortsl2561gain);
  SerialMon.println("  mqttsensorstsl2561lux: " + mqttsensorstsl2561lux);
  SerialMon.println("  sensorsenablepir: " + sensorsenablepir);
  SerialMon.println("  sensorpirpin: " + sensorpirpin);
  SerialMon.println("  mqttsensorspir: " + mqttsensorspir);
  SerialMon.println("  sensorsenablecputemp: " + sensorsenablecputemp);
  SerialMon.println("  mqttsensorscputemp: " + mqttsensorscputemp);
  SerialMon.println("  sensorsenablecpufreq: " + sensorsenablecpufreq);
  SerialMon.println("  mqttsensorscpufreq: " + mqttsensorscpufreq);
  SerialMon.println("  sensorsenablefreeram: " + sensorsenablefreeram);
  SerialMon.println("  mqttsensorsfreeram: " + mqttsensorsfreeram);
  SerialMon.println("  sensorsenablehall: " + sensorsenablehall);
  SerialMon.println("  mqttsensorshall: " + mqttsensorshall);
  SerialMon.println("  mqttmodeminfo: " + mqttmodeminfo);
  SerialMon.println("  mqttmodemccid: " + mqttmodemccid);
  SerialMon.println("  mqttmodemimsi: " + mqttmodemimsi);
  SerialMon.println("  mqttmodemoperator: " + mqttmodemoperator);
  SerialMon.println("  mqttmodemsignalquality: " + mqttmodemsignalquality);
  SerialMon.println("  mqttmodempublicip: " + mqttmodempublicip);
  SerialMon.println("  mqttbatterypercentage: " + mqttbatterypercentage);
  SerialMon.println("  mqttbatteryvoltage: " + mqttbatteryvoltage);
  SerialMon.println("  mqttbatterystatus: " + mqttbatterystatus);
  SerialMon.println("  mqttlocationtype: " + mqttlocationtype);
  SerialMon.println("  mqttlocationlatitude: " + mqttlocationlatitude);
  SerialMon.println("  mqttlocationlongitude: " + mqttlocationlongitude);
  SerialMon.println("  mqttlocationspeed: " + mqttlocationspeed);
  SerialMon.println("  mqttlocationaltitude: " + mqttlocationaltitude);
  SerialMon.println("  mqttlocationaccuracy: " + mqttlocationaccuracy);
  SerialMon.println("  ftpenable: " + ftpenable);
  SerialMon.println("  ftpuser: " + ftpuser);
  SerialMon.println("  ftppass: " + ftppass);
  SerialMon.println("  bluetoothenable: " + bluetoothenable);
  SerialMon.println("  bluetoothname: " + bluetoothname);
  SerialMon.println("  bluetoothobdidentifier: " + bluetoothobdidentifier);
  SerialMon.println("  bluetoothobdmqttobdpayload: " + bluetoothobdmqttobdpayload);
  SerialMon.println("  bluetoothobdmqttobddebug: " + bluetoothobdmqttobddebug);
  SerialMon.println("  bluetoothobdmqttengineload: " + bluetoothobdmqttengineload);
  SerialMon.println("  bluetoothobdmqttenginecoolanttemp: " + bluetoothobdmqttenginecoolanttemp);
  SerialMon.println("  bluetoothobdmqttshorttermfueltrimbank1: " + bluetoothobdmqttshorttermfueltrimbank1);
  SerialMon.println("  bluetoothobdmqttlongtermfueltrimbank1: " + bluetoothobdmqttlongtermfueltrimbank1);
  SerialMon.println("  bluetoothobdmqttshorttermfueltrimbank2: " + bluetoothobdmqttshorttermfueltrimbank2);
  SerialMon.println("  bluetoothobdmqttlongtermfueltrimbank2: " + bluetoothobdmqttlongtermfueltrimbank2);
  SerialMon.println("  bluetoothobdmqttfuelpressure: " + bluetoothobdmqttfuelpressure);
  SerialMon.println("  bluetoothobdmqttmanifoldpressure: " + bluetoothobdmqttmanifoldpressure);
  SerialMon.println("  bluetoothobdmqttrpm: " + bluetoothobdmqttrpm);
  SerialMon.println("  bluetoothobdmqttkph: " + bluetoothobdmqttkph);
  SerialMon.println("  bluetoothobdmqttmph: " + bluetoothobdmqttmph);
  SerialMon.println("  bluetoothobdmqtttimingadvance: " + bluetoothobdmqtttimingadvance);
  SerialMon.println("  bluetoothobdmqttintakeairtemp: " + bluetoothobdmqttintakeairtemp);
  SerialMon.println("  bluetoothobdmqttmafrate: " + bluetoothobdmqttmafrate);
  SerialMon.println("  bluetoothobdmqttthrottle: " + bluetoothobdmqttthrottle);
  SerialMon.println("  bluetoothobdmqttauxinputstatus: " + bluetoothobdmqttauxinputstatus);
  SerialMon.println("  bluetoothobdmqttruntime: " + bluetoothobdmqttruntime);
  SerialMon.println("  bluetoothobdmqttdisttravelwithmil: " + bluetoothobdmqttdisttravelwithmil);
  SerialMon.println("  bluetoothobdmqttfuelrailpressure: " + bluetoothobdmqttfuelrailpressure);
  SerialMon.println("  bluetoothobdmqttfuelrailguagepressure: " + bluetoothobdmqttfuelrailguagepressure);
  SerialMon.println("  bluetoothobdmqttcommandedegr: " + bluetoothobdmqttcommandedegr);
  SerialMon.println("  bluetoothobdmqttegrerror: " + bluetoothobdmqttegrerror);
  SerialMon.println("  bluetoothobdmqttcommandedevappurge: " + bluetoothobdmqttcommandedevappurge);
  SerialMon.println("  bluetoothobdmqttfuellevel: " + bluetoothobdmqttfuellevel);
  SerialMon.println("  bluetoothobdmqttwarmupssincecodescleared: " + bluetoothobdmqttwarmupssincecodescleared);
  SerialMon.println("  bluetoothobdmqttdistsincecodescleared: " + bluetoothobdmqttdistsincecodescleared);
  SerialMon.println("  bluetoothobdmqttevapsysvappressure: " + bluetoothobdmqttevapsysvappressure);
  SerialMon.println("  bluetoothobdmqttabsbaropressure: " + bluetoothobdmqttabsbaropressure);
  SerialMon.println("  bluetoothobdmqttcattempb1s1: " + bluetoothobdmqttcattempb1s1);
  SerialMon.println("  bluetoothobdmqttcattempb2s1: " + bluetoothobdmqttcattempb2s1);
  SerialMon.println("  bluetoothobdmqttcattempb1s2: " + bluetoothobdmqttcattempb1s2);
  SerialMon.println("  bluetoothobdmqttcattempb2s2: " + bluetoothobdmqttcattempb2s2);
  SerialMon.println("  bluetoothobdmqttctrlmodvoltage: " + bluetoothobdmqttctrlmodvoltage);
  SerialMon.println("  bluetoothobdmqttabsload: " + bluetoothobdmqttabsload);
  SerialMon.println("  bluetoothobdmqttcommandedairfuelratio: " + bluetoothobdmqttcommandedairfuelratio);
  SerialMon.println("  bluetoothobdmqttrelativethrottle: " + bluetoothobdmqttrelativethrottle);
  SerialMon.println("  bluetoothobdmqttambientairtemp: " + bluetoothobdmqttambientairtemp);
  SerialMon.println("  bluetoothobdmqttabsthrottleposb: " + bluetoothobdmqttabsthrottleposb);
  SerialMon.println("  bluetoothobdmqttabsthrottleposc: " + bluetoothobdmqttabsthrottleposc);
  SerialMon.println("  bluetoothobdmqttabsthrottleposd: " + bluetoothobdmqttabsthrottleposd);
  SerialMon.println("  bluetoothobdmqttabsthrottlepose: " + bluetoothobdmqttabsthrottlepose);
  SerialMon.println("  bluetoothobdmqttabsthrottleposf: " + bluetoothobdmqttabsthrottleposf);
  SerialMon.println("  bluetoothobdmqttcommandedthrottleactuator: " + bluetoothobdmqttcommandedthrottleactuator);
  SerialMon.println("  bluetoothobdmqtttimerunwithmil: " + bluetoothobdmqtttimerunwithmil);
  SerialMon.println("  bluetoothobdmqtttimesincecodescleared: " + bluetoothobdmqtttimesincecodescleared);
  SerialMon.println("  bluetoothobdmqttmaxmafrate: " + bluetoothobdmqttmaxmafrate);
  SerialMon.println("  bluetoothobdmqttethonolpercent: " + bluetoothobdmqttethonolpercent);
  SerialMon.println("  bluetoothobdmqttabsevapsysvappressure: " + bluetoothobdmqttabsevapsysvappressure);
  SerialMon.println("  bluetoothobdmqttevapsysvappressure2: " + bluetoothobdmqttevapsysvappressure2);
  SerialMon.println("  bluetoothobdmqttabsfuelrailpressure: " + bluetoothobdmqttabsfuelrailpressure);
  SerialMon.println("  bluetoothobdmqttrelativepedalpos: " + bluetoothobdmqttrelativepedalpos);
  SerialMon.println("  bluetoothobdmqtthybridbatlife: " + bluetoothobdmqtthybridbatlife);
  SerialMon.println("  bluetoothobdmqttoiltemp: " + bluetoothobdmqttoiltemp);
  SerialMon.println("  bluetoothobdmqttfuelinjecttiming: " + bluetoothobdmqttfuelinjecttiming);
  SerialMon.println("  bluetoothobdmqttfuelrate: " + bluetoothobdmqttfuelrate);
  SerialMon.println("  bluetoothobdmqttdemandedtorque: " + bluetoothobdmqttdemandedtorque);
  SerialMon.println("  bluetoothobdmqtttorque: " + bluetoothobdmqtttorque);
  SerialMon.println("  bluetoothobdmqttreferencetorque: " + bluetoothobdmqttreferencetorque);
  SerialMon.println("  bluetoothobdmqttbatteryvoltage: " + bluetoothobdmqttbatteryvoltage);
  SerialMon.println("  bluetoothobdmqttvin: " + bluetoothobdmqttvin);
  SerialMon.println("  mqttuptime: " + mqttuptime);
  SerialMon.println("  mqttversion: " + mqttversion);
  SerialMon.println("  dynamicfrequency: " + dynamicfrequency);
  SerialMon.println("  lowpowermodeonbattery: " + lowpowermodeonbattery);
  #endif
}

// Initialize the modem
#if DEBUG
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem, 0);

// Prepare MQTT
PubSubClient mqtt(client);

void setup() {
  // Increase the CPU frequency to 240MHz
  setCpuFrequencyMhz(240);

  // Set the console baud rate
  SerialMon.begin(UART_BAUD);
  delay(10);

  // Set the modem baud rate
  SerialAT.begin(UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

  // Status LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  /*
    MODEM_PWRKEY IO:4 The power-on signal of the modem must be sent to it,
    otherwise the modem will not reply when a command is sent
  */
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(300); // Needed delay
  digitalWrite(MODEM_PWRKEY, LOW);

  /*
    MODEM_FLIGHT IO:25 modem flight mode control,
    needed to enable modem, this pin must be set to high
  */
  pinMode(MODEM_FLIGHT, OUTPUT);
  digitalWrite(MODEM_FLIGHT, HIGH);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    #if DEBUG
    SerialMon.println("An error has occurred while mounting SPIFFS");
  } else {
    SerialMon.println("SPIFFS mounted successfully");
    #endif
  }

  // Load the configuration
  loadConfig();
  delay(1000);

  // WiFi AP
  WiFi.softAP(stringToChar(apssid), stringToChar(appassword));

  // FTP
  if (ftpenable == "true") {
    initFTP();
  }

  // Configure MQTT
  mqtt.setServer(mqttaddress.c_str(), stringToInt(mqttport));

  // OTA
  AsyncElegantOTA.begin(&server);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  server.serveStatic("/", SPIFFS, "/");

  // Respond to config changes
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();

    DynamicJsonDocument doc(8192);
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        for (int i = 0; i < (sizeof(configIDs) / sizeof(configIDs[0])); i++) {
          if (p->name() == configIDs[i]) {
            doc[p->name()] = p->value();
          }
        }
      }
    }

    writeFile(SPIFFS, "/config.json", doc.as<String>().c_str());

    request->send(200, "text/plain", "Configuration saved. Restarting...");
    delay(3000);
    ESP.restart();
  });

  // Respond to restart button press
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Restarting...");
    delay(3000);
    ESP.restart();
  });
  
  server.begin();

  // OBD
  if (bluetoothenable == "true") {
    initOBD();
  }
}

void initMQTT() {
    if (initialized) {
      if (!mqtt.connected()) {
          #if DEBUG
          SerialMon.println("MQTT not connected");
          #endif

          mqtt.connect(stringToChar(mqttclientname), stringToChar(mqttusername), stringToChar(mqttpassword));
      }
      #if DEBUG
      else {
          SerialMon.println("MQTT connected");
      }
      #endif
    }
}

// Package up the provided data and send it to the MQTT broker
void packageAndSendMQTT(String value, String topic) {
    if (initialized) {    
      String fullTopic = mqttclientname + (String)"/" + topic;

      mqtt.publish(stringToChar(fullTopic), stringToChar(value));
    }
}

// Uptime
void getUptime() {
    packageAndSendMQTT(String(esp_timer_get_time() / 1000000), mqttuptime);
}

// Version
void getVersion() {
    String version = ESP.getSketchMD5();

    // Send the version to the MQTT broker
    packageAndSendMQTT(version, mqttversion);

    // Save the version to a json file
    DynamicJsonDocument doc(64);
    deserializeJson(doc, readFile(SPIFFS, "/version.json"));
    // if the version is the same, don't send it
    if (doc["version"] != version) {
      DynamicJsonDocument doc(64);
      doc["version"] = version;
      writeFile(SPIFFS, "/version.json", doc.as<String>().c_str());
    }
}

void loop() {
  if ((ftpenable == "false") || (ftp.countConnections() == 0)) {
    // Initialize the modem
    initModem();

    // Connect to the cellular network
    initNetwork();

    // Get network time
    getNetworkTime();

    // Connect to an MQTT broker
    initMQTT();

    // Uptime
    getUptime();

    // Version
    getVersion();

    // Get battery info
    batteryInfo();

    // Get network info
    getNetInfo();

    // Get location info
    getLocationInfo();

    // Sensors
    if (sensorsenable == "true") {
      initSensors();
      readSensors();
    }

    // OBD
    if (bluetoothenable == "true") {
      readOBD();
    }
  }

  // FTP
  if (ftpenable == "true") {
    loopFTP();
  }
}