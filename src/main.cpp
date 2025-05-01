#include "globals.h"
#include "modem.h"
#include "sensors.h"
#include "html.h"
#include "ftp.h"

// WiFi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
AsyncWebServer server(80);

// BLE
#include <NimBLEDevice.h>

// Configuration variables
String apn, apnusername, apnpassword, simpin, timezone, dst, apssid, appassword, appowersaving, mqttaddress, mqttport, mqttclientname, mqttusername, mqttpassword, locationgnssmode, locationdpo, sensorsenable, sensorsenablebme280, sensorbme280i2caddress, mqttsensorsbme280temperature, mqttsensorsbme280pressure, mqttsensorsbme280humidity, sensorsenabletsl2561, sensortsl2561i2caddress, sensortsl2561gain, mqttsensorstsl2561lux, sensorsenablepir, sensorpirpin, mqttsensorspir, sensorsenablecputemp, mqttsensorscputemp, sensorsenablecpufreq, mqttsensorscpufreq, sensorsenablefreeram, mqttsensorsfreeram, sensorsenablehall, mqttsensorshall, mqttmodeminfo, mqttmodemccid, mqttmodemimsi, mqttmodemoperator, mqttmodemsignalquality, mqttmodempublicip, mqttbatterypercentage, mqttbatteryvoltage, mqttbatterystatus, mqttlocationtype, mqttlocationlatitude, mqttlocationlongitude, mqttlocationspeed, mqttlocationaltitude, mqttlocationaccuracy, ftpenable, ftpuser, ftppass, bluetoothibeaconenable, bluetoothibeaconuuid, mqttuptime, mqttversion, dynamicfrequency, lowpowermodeonbattery;
int connectionRetries = 0;

// Search for parameter in HTTP POST request
const String configIDs[] = {"apn", "apnusername", "apnpassword", "simpin", "timezone", "dst", "apssid", "appassword", "appowersaving", "mqttaddress", "mqttport", "mqttclientname", "mqttusername", "mqttpassword", "locationgnssmode", "locationdpo", "sensorsenable", "sensorsenablebme280", "sensorbme280i2caddress", "mqttsensorsbme280temperature", "mqttsensorsbme280pressure", "mqttsensorsbme280humidity", "sensorsenabletsl2561", "sensortsl2561i2caddress", "sensortsl2561gain", "mqttsensorstsl2561lux", "sensorsenablepir", "sensorpirpin", "mqttsensorspir", "sensorsenablecputemp", "mqttsensorscputemp", "sensorsenablecpufreq", "mqttsensorscpufreq", "sensorsenablefreeram", "mqttsensorsfreeram", "sensorsenablehall", "mqttsensorshall", "mqttmodeminfo", "mqttmodemccid", "mqttmodemimsi", "mqttmodemoperator", "mqttmodemsignalquality", "mqttmodempublicip", "mqttbatterypercentage", "mqttbatteryvoltage", "mqttbatterystatus", "mqttlocationtype", "mqttlocationlatitude", "mqttlocationlongitude", "mqttlocationspeed", "mqttlocationaltitude", "mqttlocationaccuracy", "ftpenable", "ftpuser", "ftppass", "bluetoothibeaconenable", "bluetoothibeaconuuid", "mqttuptime", "mqttversion", "dynamicfrequency", "lowpowermodeonbattery"};

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
  timezone = doc["timezone"] | "7200";
  dst = doc["dst"] | "true";
  apssid = doc["apssid"] | "Car Assistant";
  appassword = doc["appassword"] | "12345678";
  appowersaving = doc["appowersaving"] | "true";
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
  bluetoothibeaconenable = doc["bluetoothibeaconenable"] | "false";
  bluetoothibeaconuuid = doc["bluetoothibeaconuuid"] | "12345678-1234-1234-1234-123456789abc";
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
  SerialMon.println("  timezone: " + timezone);
  SerialMon.println("  dst: " + dst);
  SerialMon.println("  apssid: " + apssid);
  SerialMon.println("  appassword: " + appassword);
  SerialMon.println("  appowersaving: " + appowersaving);
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
  SerialMon.println("  bluetoothibeaconenable: " + bluetoothibeaconenable);
  SerialMon.println("  bluetoothibeaconuuid: " + bluetoothibeaconuuid);
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

void rebootDevice() {
  #if DEBUG
  SerialMon.println("Rebooting device...");
  #endif

  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(300); // Needed delay
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(300);

  modem.restart();
  ESP.restart();
}

// Prepare MQTT
PubSubClient mqtt(client);

// Receive MQTT commands
void callback(char* topic, byte* payload, unsigned int length) {
  String newTopic = topic;
  payload[length] = '\0';
  String newPayload = String((char *)payload);
  if (newTopic == mqttclientname + (String)"/commands") {
    if (newPayload == "restart") {
      rebootDevice();
    }
  }
}

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
  mqtt.setCallback(callback);

  // OTA
  ElegantOTA.begin(&server);

  // BLE
  if (bluetoothibeaconenable == "true") {
    NimBLEDevice::init("");
  
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    NimBLEAdvertisementData advData;
  
    std::vector<uint8_t> mfgData;
  
    // Apple Manufacturer ID (0x004C little-endian)
    mfgData.push_back(0x4C);
    mfgData.push_back(0x00);
  
    // iBeacon Type and Length
    mfgData.push_back(0x02); // Type
    mfgData.push_back(0x15); // Length
  
    // UUID as string
    NimBLEUUID uuid(stringToChar(bluetoothibeaconuuid));

    // Convert to 128-bit representation if necessary
    uuid.to128();

    const uint8_t* uuidBytes = uuid.getValue();
    mfgData.insert(mfgData.end(), uuidBytes, uuidBytes + 16);
  
    // Major: 100 (0x0064)
    mfgData.push_back(0x00);
    mfgData.push_back(0x64);
  
    // Minor: 1 (0x0001)
    mfgData.push_back(0x00);
    mfgData.push_back(0x01);
  
    // Tx Power
    mfgData.push_back(0xC5); // -59 dBm
  
    advData.setFlags(0x04); // BR/EDR Not Supported
    advData.setManufacturerData(mfgData);
  
    pAdvertising->setAdvertisementData(advData);
    pAdvertising->start();
  }

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
      const AsyncWebParameter* p = request->getParam(i);
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
}

void initMQTT() {
    if (initialized) {
      if (!mqtt.connected()) {
        #if DEBUG
        SerialMon.println("MQTT not connected");
        #endif

        connectionRetries++;
        if (connectionRetries > 100) {
            rebootDevice();
        }

        mqtt.connect(stringToChar(mqttclientname), stringToChar(mqttusername), stringToChar(mqttpassword));
      } else {
        #if DEBUG
        SerialMon.println("MQTT connected");
        #endif
        connectionRetries = 0;

        mqtt.subscribe(stringToChar(mqttclientname + (String)"/commands"));
      }
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

// Change the CPU frequency
void setCPUFrequency(int frequency) {
    WiFi.enableAP(false);
    delay(100);
    
    setCpuFrequencyMhz(frequency);

    WiFi.enableAP(true);
}

// Version
void getVersion() {
    String version;

    #ifdef VERSION
    version = VERSION;
    #else
    version = ESP.getSketchMD5();
    #endif

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
    mqtt.loop();

    // Uptime
    getUptime();

    // Version
    getVersion();

    // Get battery info
    batteryInfo();

    // WiFi Hotspot
    if ((!charging) || (stationary)) {
      WiFi.enableAP(false);
    } else {
      WiFi.enableAP(true);
    }

    // Get network info
    getNetInfo();

    // Get location info
    getLocationInfo();

    // Sensors
    if (sensorsenable == "true") {
      initSensors();
      readSensors();
    }

    // OTA
    ElegantOTA.loop();
  }

  // FTP
  if (ftpenable == "true") {
    loopFTP();
  }
}