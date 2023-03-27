#include "modem.h"

bool initialized = false;

bool timeSet = false;

String publicIP;

// Initialize the modem
void initModem() {
    if (!initialized) {
        #if DEBUG
        SerialMon.println("\nInitializing modem...");
        #endif
        if (!modem.init()) {
            #if DEBUG
            SerialMon.println("Failed to initiate the modem, retrying...");
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
        if (simpin != "" && modem.getSimStatus() != 3) {
            modem.simUnlock(stringToChar(simpin));
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
        SerialMon.println("Failed to get public IP");
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
        SerialMon.println("\nWaiting for network...");
        #endif
        if (!modem.waitForNetwork(600000L)) {
            delay(10000);
            return;
        }

        #if DEBUG
        if (modem.isNetworkConnected()) {
            SerialMon.println("Network connected");
        }

        SerialMon.println((String)"\nConnecting to: " + apn);
        #endif
        if (!modem.gprsConnect(stringToChar(apn), stringToChar(apnusername), stringToChar(apnpassword))) {
            delay(10000);
            return;
        }

        #if DEBUG
        if (modem.isGprsConnected()) {
            SerialMon.println("Connected to the cellular network");
        } else {
            SerialMon.println("Failed to connect to the cellular network");
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
        float offset = 0;
        int dstOffset = 0;

        if (dst == "true") {
            offset = stringToInt(timezone) + 3600;
            dstOffset = 3600;
        } else {
            offset = stringToInt(timezone);
        }

        for (int8_t i = 5; i; i--) {
            #if DEBUG
            SerialMon.println("Requesting current network time");
            #endif

            if (modem.getNetworkTime(&netyear, &netmonth, &netday, &nethour, &netmin, &netsec, &offset)) {
                #if DEBUG
                SerialMon.println((String)"Year: " + netyear + "\tMonth: " + netmonth + "\tDay: " + netday);
                SerialMon.println((String)"Hour: " + nethour + "\tMinute: " + netmin + "\tSecond: " + netsec);
                SerialMon.println((String)"Timezone: " + offset);
                #endif
                configTime(stringToInt(timezone), dstOffset, "pool.ntp.org", "time.apple.com", "time.google.com");
            } else {
                #if DEBUG
                SerialMon.println("Couldn't get network time, retrying in 15s.");
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

        // Send the voltage to the MQTT broker
        packageAndSendMQTT(String(voltage), mqttbatteryvoltage);

        // Calculate percentage
        output = ((voltage - battery_min) / (battery_max - battery_min)) * 100;

        // Send the percentage to the MQTT broker
        if (output < 100) {
            packageAndSendMQTT(String(output), mqttbatterypercentage);
        } else {
            packageAndSendMQTT("100", mqttbatterypercentage);
        }

        uint32_t cpuFrequency = getCpuFrequencyMhz();
        // Check if the battery is charging
        if (analogRead(SOLAR_INDICATOR) != 0) { // Solar panel connector
            packageAndSendMQTT("Charging", mqttbatterystatus);
            digitalWrite(LED_PIN, HIGH);

            // Increase the CPU frequency to 240MHz
            if (cpuFrequency != 240 && dynamicfrequency == "true") {
                setCpuFrequencyMhz(240);
            }
        } else if (analogRead(USB_INDICATOR) == 0) { // USB connectors
            packageAndSendMQTT("USB Charging", mqttbatterystatus);
            digitalWrite(LED_PIN, HIGH);

            // Increase the CPU frequency to 240MHz
            if (cpuFrequency != 240 && dynamicfrequency == "true") {
                setCpuFrequencyMhz(240);
            }
        } else {
            packageAndSendMQTT("Discharging", mqttbatterystatus);
            digitalWrite(LED_PIN, LOW);

            // Decrease the CPU frequency to 160MHz/80MHz to save power
            if (dynamicfrequency == "true") {
                if (lowpowermodeonbattery == "true") {
                    if (cpuFrequency != 80) {
                        setCpuFrequencyMhz(80);
                    }
                } else {
                    if (cpuFrequency != 160) {
                        setCpuFrequencyMhz(160);
                    }
                }
            }
        }
    }
}

void getNetInfo() {
    if (initialized && mqtt.connected()) {
        packageAndSendMQTT(String(modem.getModemInfo()), mqttmodeminfo);

        packageAndSendMQTT(String(modem.getSimCCID()), mqttmodemccid);

        packageAndSendMQTT(String(modem.getIMSI()), mqttmodemimsi);

        packageAndSendMQTT(String(modem.getOperator()), mqttmodemoperator);

        packageAndSendMQTT(String(modem.getSignalQuality()), mqttmodemsignalquality);

        packageAndSendMQTT(publicIP, mqttmodempublicip);
    }
}

void getLocationInfo() {
    if (initialized && mqtt.connected()) {
        modem.setGNSSMode(stringToInt(locationgnssmode), stringToInt(locationdpo));
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
        SerialMon.println("Requesting your current GNSS location");
        #endif

        if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2, &year2, &month2, &day2, &hour2, &min2, &sec2)) {
            #if DEBUG
            SerialMon.println("Latitude: " + String(lat2, 8) + "\tLongitude: " + String(lon2, 8));
            SerialMon.println("Speed: " + String(speed2) + "\tAltitude: " + String(alt2));
            SerialMon.println("Visible Satellites: " + String(vsat2) + "\tUsed Satellites: " + String(usat2));
            SerialMon.println("Accuracy: " + String(accuracy2));
            SerialMon.println("Year: " + String(year2) + "\tMonth: " + String(month2) + "\tDay: " + String(day2));
            SerialMon.println("Hour: " + String(hour2) + "\tMinute: " + String(min2) + "\tSecond: " + String(sec2));
            #endif

            packageAndSendMQTT("GNSS", mqttlocationtype);
            packageAndSendMQTT(String(lat2, 8), mqttlocationlatitude);
            packageAndSendMQTT(String(lon2, 8), mqttlocationlongitude);
            packageAndSendMQTT(String(speed2), mqttlocationspeed);
            packageAndSendMQTT(String(alt2), mqttlocationaltitude);
            packageAndSendMQTT(String(accuracy2), mqttlocationaccuracy);
        } else if (modem.getGsmLocation(&lat2, &lon2, &accuracy2, &year2, &month2, &day2, &hour2, &min2, &sec2)) {
            #if DEBUG
            SerialMon.println("Falling back to a cellular location");
            SerialMon.println("Latitude: " + String(lat2, 8) + "\tLongitude: " + String(lon2, 8));
            SerialMon.println("Accuracy: " + String(accuracy2));
            SerialMon.println("Year: " + String(year2) + "\tMonth: " + String(month2) + "\tDay: " + String(day2));
            SerialMon.println("Hour: " + String(hour2) + "\tMinute: " + String(min2) + "\tSecond: " + String(sec2));
            #endif

            packageAndSendMQTT("Cellular", mqttlocationtype);
            packageAndSendMQTT(String(lat2, 8), mqttlocationlatitude);
            packageAndSendMQTT(String(lon2, 8), mqttlocationlongitude);
            packageAndSendMQTT(String(accuracy2), mqttlocationaccuracy);
        } else {
            #if DEBUG
            SerialMon.println("Couldn't get your location");
            #endif

            packageAndSendMQTT("None", mqttlocationtype);
        }
    }
}