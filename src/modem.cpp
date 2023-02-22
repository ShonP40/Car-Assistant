#include "modem.h"

bool initialized = false;

bool timeSet = false;

String publicIP;

// Initialize the modem
void initModem() {
    if (!initialized) {
        #if DEBUG
        Serial.println("\nInitializing modem...");
        #endif
        if (!modem.init()) {
            #if DEBUG
            Serial.println("Failed to initiate the modem, retrying...");
            #endif
            initialized = false;
            return;
        } else {
            initialized = true;
        }

        /*  Preferred mode selection : AT+CNMP
            2  – Automatic
            13 – GSM Only
            14 – WCDMA Only
            38 – LTE Only
            59 – TDS-CDMA Only
            9  – CDMA Only
            10 – EVDO Only
            19 – GSM+WCDMA Only
            22 – CDMA+EVDO Only
            48 – Any but LTE
            60 – GSM+TDSCDMA Only
            63 – GSM+WCDMA+TDSCDMA Only
            67 – CDMA+EVDO+GSM+WCDMA+TDSCDMA Only
            39 – GSM+WCDMA+LTE Only
            51 – GSM+LTE Only
            54 – WCDMA+LTE Only
        */

        modem.setNetworkMode(2);

        // Unlock the SIM card with a PIN if needed
        if (GSM_PIN && modem.getSimStatus() != 3) {
            modem.simUnlock(GSM_PIN);
        }

        delay(1000);
    }
}

void getPublicIP() {
    if (client.connect("api.ipify.org", 80)) {
        client.println("GET / HTTP/1.0");
        client.println("Host: api.ipify.org");
        client.println();
    } else {
        #if DEBUG
        Serial.println("Failed to get public IP");
        #endif
    }
    delay(5000);
    String line;
    while(client.available()) {
        line = client.readStringUntil('\n');
    }
    publicIP = line;
}

// Connect to the cellular network
void initNetwork() {
    if (!modem.isGprsConnected() && initialized) {
        #if DEBUG
        Serial.println("\nWaiting for network...");
        #endif
        if (!modem.waitForNetwork(600000L)) {
            delay(10000);
            return;
        }

        #if DEBUG
        if (modem.isNetworkConnected()) {
            Serial.println("Network connected");
        }

        Serial.println((String)"\nConnecting to: " + APN);
        #endif
        if (!modem.gprsConnect(APN, CELL_USER, CELL_PASS)) {
            delay(10000);
            return;
        }

        #if DEBUG
        if (modem.isGprsConnected()) {
            Serial.println("Connected to the cellular network");
        } else {
            Serial.println("Failed to connect to the cellular network");
        }
        #endif

        getPublicIP();
    }
}

// Get network time
void getNetworkTime() {
    if (!timeSet && initialized) {
        int netyear = 0;
        int netmonth = 0;
        int netday = 0;
        int nethour = 0;
        int netmin = 0;
        int netsec = 0;
        float timezone = 0;

        for (int8_t i = 5; i; i--) {
            #if DEBUG
            Serial.println("Requesting current network time");
            #endif

            if (modem.getNetworkTime(&netyear, &netmonth, &netday, &nethour, &netmin, &netsec, &timezone)) {
                #if DEBUG
                Serial.println((String)"Year: " + netyear + "\tMonth: " + netmonth + "\tDay: " + netday);
                Serial.println((String)"Hour: " + nethour + "\tMinute: " + netmin + "\tSecond: " + netsec);
                Serial.println((String)"Timezone: " + timezone);
                #endif
                configTime(timezone, 0, "pool.ntp.org", "time.apple.com", "time.google.com");
            } else {
                #if DEBUG
                Serial.println("Couldn't get network time, retrying in 15s.");
                #endif
                delay(15000);
            }
        }

        timeSet = true;
    }
}

// Get and report the battery percentage
void batteryInfo() {
    if (initialized && mqtt.connected()) {
        float voltage = 0.0;            // Calculated voltage
        float output = 0.0;             // Output value
        const float battery_max = 4.20; // Maximum battery voltage
        const float battery_min = 3.0;  // Minimum battery voltage

        // Calculate the voltage
        voltage = modem.getBattVoltage() / 1000.0;

        // Print the voltage
        packageAndSendMQTT(String(voltage), MQTT_BATTERY_VOLTAGE);

        // Calculate percentage
        output = ((voltage - battery_min) / (battery_max - battery_min)) * 100;

        // Print the percentage
        if (output < 100) {
            packageAndSendMQTT(String(output), MQTT_BATTERY_PERCENTAGE);
        } else {
            packageAndSendMQTT("100", MQTT_BATTERY_PERCENTAGE);
        }

        // Check if the battery is charging
        if (analogRead(SOLAR_INDICATOR) != 0) { // Solar panel connector
            packageAndSendMQTT("Charging", MQTT_BATTERY_STATUS);
            digitalWrite(LED_PIN, HIGH);
        } else if (analogRead(USB_INDICATOR) == 0) { // USB connectors
            packageAndSendMQTT("USB Charging", MQTT_BATTERY_STATUS);
            digitalWrite(LED_PIN, HIGH);
        } else {
            packageAndSendMQTT("Discharging", MQTT_BATTERY_STATUS);
            digitalWrite(LED_PIN, LOW);
        }
    }
}

void getNetInfo() {
    if (initialized && mqtt.connected()) {
        packageAndSendMQTT(String(modem.getModemInfo()), MQTT_MODEM_INFO);

        packageAndSendMQTT(String(modem.getSimCCID()), MQTT_CCID);

        packageAndSendMQTT(String(modem.getIMSI()), MQTT_IMSI);

        packageAndSendMQTT(String(modem.getOperator()), MQTT_OPERATOR);

        packageAndSendMQTT(String(modem.getSignalQuality()), MQTT_SIGNAL_QUALITY);

        packageAndSendMQTT(publicIP, MQTT_PUBLIC_IP);
    }
}

void getLocationInfo() {
    if (initialized && mqtt.connected()) {
        modem.setGNSSMode(GNSS_MODE, DPO);
        delay(1000);

        modem.enableGPS();
        delay(2000);

        float lat2      = 0;
        float lon2      = 0;
        float speed2    = 0;
        float alt2      = 0;
        int   vsat2     = 0;
        int   usat2     = 0;
        float accuracy2 = 0;
        int   year2     = 0;
        int   month2    = 0;
        int   day2      = 0;
        int   hour2     = 0;
        int   min2      = 0;
        int   sec2      = 0;

        #if DEBUG
        Serial.println("Requesting your current GNSS location");
        #endif

        if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2, &year2, &month2, &day2, &hour2, &min2, &sec2)) {
            #if DEBUG
            Serial.println("Latitude: " + String(lat2, 8) + "\tLongitude: " + String(lon2, 8));
            Serial.println("Speed: " + String(speed2) + "\tAltitude: " + String(alt2));
            Serial.println("Visible Satellites: " + String(vsat2) + "\tUsed Satellites: " + String(usat2));
            Serial.println("Accuracy: " + String(accuracy2));
            Serial.println("Year: " + String(year2) + "\tMonth: " + String(month2) + "\tDay: " + String(day2));
            Serial.println("Hour: " + String(hour2) + "\tMinute: " + String(min2) + "\tSecond: " + String(sec2));
            #endif

            packageAndSendMQTT("GNSS", MQTT_LOCATION_TYPE);
            packageAndSendMQTT(String(lat2, 8), MQTT_LATITUDE);
            packageAndSendMQTT(String(lon2, 8), MQTT_LONGITUDE);
            packageAndSendMQTT(String(speed2), MQTT_SPEED);
            packageAndSendMQTT(String(alt2), MQTT_ALTITUDE);
            packageAndSendMQTT(String(accuracy2), MQTT_ACCURACY);
        } else if (modem.getGsmLocation(&lat2, &lon2, &accuracy2, &year2, &month2, &day2, &hour2, &min2, &sec2)) {
            #if DEBUG
            Serial.println("Falling back to a cellular location");
            Serial.println("Latitude: " + String(lat2, 8) + "\tLongitude: " + String(lon2, 8));
            Serial.println("Accuracy: " + String(accuracy2));
            Serial.println("Year: " + String(year2) + "\tMonth: " + String(month2) + "\tDay: " + String(day2));
            Serial.println("Hour: " + String(hour2) + "\tMinute: " + String(min2) + "\tSecond: " + String(sec2));
            #endif

            packageAndSendMQTT("Cellular", MQTT_LOCATION_TYPE);
            packageAndSendMQTT(String(lat2, 8), MQTT_LATITUDE);
            packageAndSendMQTT(String(lon2, 8), MQTT_LONGITUDE);
            packageAndSendMQTT(String(accuracy2), MQTT_ACCURACY);
        } else {
            #if DEBUG
            Serial.println("Couldn't get your location");
            #endif

            packageAndSendMQTT("None", MQTT_LOCATION_TYPE);
        }
    }
}