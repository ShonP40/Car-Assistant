#include "globals.h"
#include "modem.h"
#include "sensors.h"

// WiFi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

void setup() {
  // Set the console baud rate
  SerialMon.begin(115200);
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

  // WiFi
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Welcome to Car Assistant!");
  });

  AsyncElegantOTA.begin(&server);
  
  server.begin();
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
PubSubClient mqtt(MQTT_ADDRESS, MQTT_PORT, client);
void initMQTT() {
      if (initialized) {
        if (!mqtt.connected()) {
            #if DEBUG
            Serial.println((String)"MQTT no connected");
            #endif

            mqtt.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS);
        }
        #if DEBUG
        else {
            Serial.println((String)"MQTT connected");
        }
        #endif
      }
}

// MQTT
char mqtt_send_package[150];
char mqtt_send_topic[150];
char mqtt_commands_topic[150];

// Package up the provided data and send it to the MQTT broker
void packageAndSendMQTT(String value, String topic) {
    if (initialized) {
      value.toCharArray(mqtt_send_package, value.length() + 1);
      
      String fullTopic = MQTT_CLIENT_NAME + (String)"/" + topic;
      fullTopic.toCharArray(mqtt_send_topic, fullTopic.length() + 1);

      mqtt.publish(mqtt_send_topic, mqtt_send_package);
    }
}

// Uptime
void getUptime() {
    packageAndSendMQTT(String(esp_timer_get_time() / 1000000), MQTT_ESP_UPTIME);
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
  #if SENSORS_ENABLED
  initSensors();
  readSensors();
  #endif
}