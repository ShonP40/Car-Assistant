#include "modem.h"

bool initialized = false;

bool timeSet = false;

void light_sleep(uint32_t sec) {
  esp_sleep_enable_timer_wakeup(sec * 1000000ULL);
  esp_light_sleep_start();
}

// Initialize the modem
#if DEBUG
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// Initialize the modem
void initModem() {
    if (!initialized) {
        Serial.println("\nInitializing modem...");
        if (!modem.init()) {
            Serial.println("Failed to initiate the modem, retrying...");
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

        light_sleep(1);
    }
}

// Get modem info
void modemInfo() {
    if (!modem.isGprsConnected() && initialized) {
        Serial.println((String)"\nModem Info: " + modem.getModemInfo());
    }
}

String getPublicIP() {
    TinyGsmClient client(modem, 0);
    if (client.connect("api.ipify.org", 80)) {
        client.println("GET / HTTP/1.0");
        client.println("Host: api.ipify.org");
        client.println();
    } else {
        Serial.println("Failed to get public IP");
        return String();
    }
    delay(5000);
    String line;
    while(client.available()) {
        line = client.readStringUntil('\n');
    }
    return line;
}

// Connect to the cellular network
void initNetwork() {
    if (!modem.isGprsConnected() && initialized) {
        bool res;

        Serial.println("\nWaiting for network...");
        if (!modem.waitForNetwork(600000L)) {
            light_sleep(10);
            return;
        }

        if (modem.isNetworkConnected()) {
            Serial.println("Network connected");
        }

        Serial.println((String)"\nConnecting to: " + APN);
        if (!modem.gprsConnect(APN, CELL_USER, CELL_PASS)) {
            light_sleep(10);
            return;
        }

        if (modem.isGprsConnected()) {
            Serial.println("Connected to the cellular network");
        } else {
            Serial.println("Failed to connect to the cellular network");
        }

        Serial.println((String)"\nCCID: " + modem.getSimCCID());

        Serial.println((String)"IMSI: " + modem.getIMSI());

        Serial.println((String)"Operator: " + modem.getOperator());

        Serial.println((String)"Signal quality: " + modem.getSignalQuality());

        Serial.println((String)"\nPublic IP: " + getPublicIP() + "\n");
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
            Serial.println("Requesting current network time");

            if (modem.getNetworkTime(&netyear, &netmonth, &netday, &nethour, &netmin, &netsec, &timezone)) {
                Serial.println((String)"Year: " + netyear + "\tMonth: " + netmonth + "\tDay: " + netday);
                Serial.println((String)"Hour: " + nethour + "\tMinute: " + netmin + "\tSecond: " + netsec);
                Serial.println((String)"Timezone: " + timezone);
                configTime(timezone, 0, "time.apple.com", "time.google.com");
                break;
            } else {
                Serial.println("Couldn't get network time, retrying in 15s.");
                light_sleep(15);
            }
        }

        timeSet = true;
    }
}