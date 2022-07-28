// Modem model
#define TINY_GSM_MODEM_SIM7600

#include <TinyGsmClient.h>
#include "config.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// Initialize modem
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);

void light_sleep(uint32_t sec) {
  esp_sleep_enable_timer_wakeup(sec * 1000000ULL);
  esp_light_sleep_start();
}

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Set GSM module baud rate
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
}

void loop() {
  bool res;

  DBG("Initializing modem...");
  if (!modem.init()) {
    DBG("Failed to restart modem, delaying 10s and retrying");
    return;
  }

  /*  Preferred mode selection : AT+CNMP
        2 – Automatic
        13 – GSM Only
        14 – WCDMA Only
        38 – LTE Only
        59 – TDS-CDMA Only
        9 – CDMA Only
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
  String ret;
  ret = modem.setNetworkMode(2);
  DBG("setNetworkMode:", ret);

  light_sleep(1);

  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

  // Unlock your SIM card with a PIN if needed
  if (GSM_PIN && modem.getSimStatus() != 3) {
    modem.simUnlock(GSM_PIN);
  }

  DBG("Waiting for network...");
  if (!modem.waitForNetwork(600000L)) {
    light_sleep(10);
    return;
  }

  if (modem.isNetworkConnected()) {
    DBG("Network connected");
  }

  DBG("Connecting to", APN);
  if (!modem.gprsConnect(APN, CELL_USER, CELL_PASS)) {
    light_sleep(10);
    return;
  }

  res = modem.isGprsConnected();
  DBG("Cellular status:", res ? "Connected" : "Not connected");

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String imsi = modem.getIMSI();
  DBG("IMSI:", imsi);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);

  // Get the current date and time from the cellular provider
  int netyear = 0;
  int netmonth = 0;
  int netday = 0;
  int nethour = 0;
  int netmin = 0;
  int netsec = 0;
  float timezone = 0;
  for (int8_t i = 5; i; i--) {
    DBG("Requesting current network time");
    if (modem.getNetworkTime(&netyear, &netmonth, &netday, &nethour, &netmin, &netsec,
                             &timezone)) {
      DBG("Year:", netyear, "\tMonth:", netmonth, "\tDay:", netday);
      DBG("Hour:", nethour, "\tMinute:", netmin, "\tSecond:", netsec);
      DBG("Timezone:", timezone);
      break;
    } else {
      DBG("Couldn't get network time, retrying in 15s.");
      light_sleep(15);
    }
  }
}