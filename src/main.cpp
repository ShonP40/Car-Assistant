#include "globals.h"
#include "modem.h"
#include "sensors.h"
#include "html.h"

// WiFi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

// Configuration variables
String apn, apnusername, apnpassword, simpin, apssid, appassword, mqttaddress, mqttport, mqttclientname, mqttusername, mqttpassword, locationgnssmode, locationdpo, sensorsenable, sensorsenablebme280, sensorbme280i2caddress, mqttsensorsbme280temperature, mqttsensorsbme280pressure, mqttsensorsbme280humidity, sensorsenabletsl2561, sensortsl2561i2caddress, sensortsl2561gain, mqttsensorstsl2561lux, sensorsenablepir, sensorpirpin, mqttsensorspir, mqttmodeminfo, mqttmodemccid, mqttmodemimsi, mqttmodemoperator, mqttmodemsignalquality, mqttmodempublicip, mqttbatterypercentage, mqttbatteryvoltage, mqttbatterystatus, mqttuptime, mqttlocationtype, mqttlocationlatitude, mqttlocationlongitude, mqttlocationspeed, mqttlocationaltitude, mqttlocationaccuracy;

// Search for parameter in HTTP POST request
const String configIDs[] = {"apn", "apnusername", "apnpassword", "simpin", "apssid", "appassword", "mqttaddress", "mqttport", "mqttclientname", "mqttusername", "mqttpassword", "locationgnssmode", "locationdpo", "sensorsenable", "sensorsenablebme280", "sensorbme280i2caddress", "mqttsensorsbme280temperature", "mqttsensorsbme280pressure", "mqttsensorsbme280humidity", "sensorsenabletsl2561", "sensortsl2561i2caddress", "sensortsl2561gain", "mqttsensorstsl2561lux", "sensorsenablepir", "sensorpirpin", "mqttsensorspir", "mqttmodeminfo", "mqttmodemccid", "mqttmodemimsi", "mqttmodemoperator", "mqttmodemsignalquality", "mqttmodempublicip", "mqttbatterypercentage", "mqttbatteryvoltage", "mqttbatterystatus", "mqttuptime", "mqttlocationtype", "mqttlocationlatitude", "mqttlocationlongitude", "mqttlocationspeed", "mqttlocationaltitude", "mqttlocationaccuracy"};

// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path){
  #if DEBUG
  Serial.printf("Reading file: %s\r\n", path);
  #endif

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    #if DEBUG
    Serial.println("Failed to open file for reading");
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
  Serial.printf("Deleting existing file: %s\r\n", path);

  Serial.printf("Writing file: %s\r\n", path);
  #endif

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    #if DEBUG
    Serial.println("Failed to open file for writing");
    #endif
    return;
  }
  if (file.print(message)) {
    #if DEBUG
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
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
  DynamicJsonDocument doc(1024);
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
  mqttmodeminfo = doc["mqttmodeminfo"] | "modem-info";
  mqttmodemccid = doc["mqttmodemccid"] | "ccid";
  mqttmodemimsi = doc["mqttmodemimsi"] | "imsi";
  mqttmodemoperator = doc["mqttmodemoperator"] | "operator";
  mqttmodemsignalquality = doc["mqttmodemsignalquality"] | "signal-quality";
  mqttmodempublicip = doc["mqttmodempublicip"] | "public-ip";
  mqttbatterypercentage = doc["mqttbatterypercentage"] | "battery-percentage";
  mqttbatteryvoltage = doc["mqttbatteryvoltage"] | "battery-voltage";
  mqttbatterystatus = doc["mqttbatterystatus"] | "battery-status";
  mqttuptime = doc["mqttuptime"] | "uptime";
  mqttlocationtype = doc["mqttlocationtype"] | "location-type";
  mqttlocationlatitude = doc["mqttlocationlatitude"] | "latitude";
  mqttlocationlongitude = doc["mqttlocationlongitude"] | "longitude";
  mqttlocationspeed = doc["mqttlocationspeed"] | "speed";
  mqttlocationaltitude = doc["mqttlocationaltitude"] | "altitude";
  mqttlocationaccuracy = doc["mqttlocationaccuracy"] | "location-accuracy";

  #if DEBUG
  Serial.println("Loaded configuration from config.json");
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
    Serial.println("An error has occurred while mounting SPIFFS");
  } else {
    Serial.println("SPIFFS mounted successfully");
    #endif
  }

  // Load the configuration
  loadConfig();
  delay(1000);

  // WiFi AP
  WiFi.softAP(stringToChar(apssid), stringToChar(appassword));

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

    DynamicJsonDocument doc(1024);
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
  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
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
            Serial.println("MQTT no connected");
            #endif

            mqtt.connect(stringToChar(mqttclientname), stringToChar(mqttusername), stringToChar(mqttpassword));
        }
        #if DEBUG
        else {
            Serial.println("MQTT connected");
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

void loop() {
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
}