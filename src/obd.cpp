#include "obd.h"

BluetoothSerial SerialBT;

ELM327 myELM327;

uint8_t pairedDeviceBtAddr[PAIR_MAX_DEVICES][6];

#if DEBUG
char bda_str[18];

char *bda2str(const uint8_t* bda, char *str, size_t size) {
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
    return str;
}
#endif

bool initBluetooth() {
    if (!btStart()) {
        #if DEBUG
        SerialMon.println("Failed to initialize controller");
        #endif
        return false;
    }
    
    if (esp_bluedroid_init() != ESP_OK) {
        #if DEBUG
        SerialMon.println("Failed to initialize bluedroid");
        #endif
        return false;
    }
 
    if (esp_bluedroid_enable() != ESP_OK) {
        #if DEBUG
        SerialMon.println("Failed to enable bluedroid");
        #endif
        return false;
    }
    return true;
}

void initOBD() {
    // Unpair all known Bluetooth devices
    initBluetooth();
    int count = esp_bt_gap_get_bond_device_num();
    if (!count) {
        #if DEBUG
        SerialMon.println("No bonded devices found.");
        #endif
    } else {
        #if DEBUG
        SerialMon.print("Bonded device count: "); SerialMon.println(count);
        #endif
        if (PAIR_MAX_DEVICES < count) {
            count = PAIR_MAX_DEVICES;
            #if DEBUG
            SerialMon.print("Reset bonded device count: "); SerialMon.println(count);
            #endif
        }
        esp_err_t tError =  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
        if (ESP_OK == tError) {
            for (int i = 0; i < count; i++) {
                #if DEBUG
                SerialMon.print("Found bonded device # "); SerialMon.print(i); SerialMon.print(" -> ");
                SerialMon.println(bda2str(pairedDeviceBtAddr[i], bda_str, 18));    
                #endif 
                esp_err_t tError = esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
                #if DEBUG
                if (ESP_OK == tError) {
                    SerialMon.print("Removed bonded device # "); 
                } else {
                    SerialMon.print("Failed to remove bonded device # ");
                }
                SerialMon.println(i);
                #endif
            }
        }
    }

    // Initialize the OBD scanner
    SerialBT.begin(bluetoothname, true);

    if (!SerialBT.connect(bluetoothobdidentifier)) {
        #if DEBUG
        SerialMon.println("Couldn't connect to OBD scanner - Phase 1");
        #endif
        while(1);
    }

    if (!myELM327.begin(SerialBT, DEBUG, 2000)) {
        #if DEBUG
        SerialMon.println("Couldn't connect to OBD scanner - Phase 2");
        #endif
        while(1);
    }
}

void readOBD() {
    /* uint32_t supportedPIDs_1_20(); */
    float engineLoad = myELM327.engineLoad();
    float engineCoolantTemp = myELM327.engineCoolantTemp();
    float shortTermFuelTrimBank_1 = myELM327.shortTermFuelTrimBank_1();
    float longTermFuelTrimBank_1 = myELM327.longTermFuelTrimBank_1();
    float shortTermFuelTrimBank_2 = myELM327.shortTermFuelTrimBank_2();
    float longTermFuelTrimBank_2 = myELM327.longTermFuelTrimBank_2();
    float fuelPressure = myELM327.fuelPressure();
    uint8_t manifoldPressure = myELM327.manifoldPressure();
    float rpm = myELM327.rpm();
    int32_t kph = myELM327.kph();
    float mph = myELM327.mph();
    float timingAdvance = myELM327.timingAdvance();
    float intakeAirTemp = myELM327.intakeAirTemp();
    float mafRate = myELM327.mafRate();
    float throttle = myELM327.throttle();
    bool auxInputStatus = myELM327.auxInputStatus();
    uint16_t runTime = myELM327.runTime();
    
    /* uint32_t supportedPIDs_21_40(); */
    uint16_t distTravelWithMIL = myELM327.distTravelWithMIL();
    float fuelRailPressure = myELM327.fuelRailPressure();
    float fuelRailGuagePressure = myELM327.fuelRailGuagePressure();
    float commandedEGR = myELM327.commandedEGR();
    float egrError = myELM327.egrError();
    float commandedEvapPurge = myELM327.commandedEvapPurge();
    float fuelLevel = myELM327.fuelLevel();
    uint8_t warmUpsSinceCodesCleared = myELM327.warmUpsSinceCodesCleared();
    uint16_t distSinceCodesCleared = myELM327.distSinceCodesCleared();
    float evapSysVapPressure = myELM327.evapSysVapPressure();
    uint8_t absBaroPressure = myELM327.absBaroPressure();
    float catTempB1S1 = myELM327.catTempB1S1();
    float catTempB2S1 = myELM327.catTempB2S1();
    float catTempB1S2 = myELM327.catTempB1S2();
    float catTempB2S2 = myELM327.catTempB2S2();

    /* uint32_t supportedPIDs_41_60(); */
    float ctrlModVoltage = myELM327.ctrlModVoltage();
    float absLoad = myELM327.absLoad();
    float commandedAirFuelRatio = myELM327.commandedAirFuelRatio();
    float relativeThrottle = myELM327.relativeThrottle();
    float ambientAirTemp = myELM327.ambientAirTemp();
    float absThrottlePosB = myELM327.absThrottlePosB();
    float absThrottlePosC = myELM327.absThrottlePosC();
    float absThrottlePosD = myELM327.absThrottlePosD();
    float absThrottlePosE = myELM327.absThrottlePosE();
    float absThrottlePosF = myELM327.absThrottlePosF();
    float commandedThrottleActuator = myELM327.commandedThrottleActuator();
    uint16_t timeRunWithMIL = myELM327.timeRunWithMIL();
    uint16_t timeSinceCodesCleared = myELM327.timeSinceCodesCleared();
    float maxMafRate = myELM327.maxMafRate();
    float ethonolPercent = myELM327.ethonolPercent();
    float absEvapSysVapPressure = myELM327.absEvapSysVapPressure();
    float evapSysVapPressure2 = myELM327.evapSysVapPressure2();
    float absFuelRailPressure = myELM327.absFuelRailPressure();
    float relativePedalPos = myELM327.relativePedalPos();
    float hybridBatLife = myELM327.hybridBatLife();
    float oilTemp = myELM327.oilTemp();
    float fuelInjectTiming = myELM327.fuelInjectTiming();
    float fuelRate = myELM327.fuelRate();

    /* uint32_t supportedPIDs_61_80(); */
    float demandedTorque = myELM327.demandedTorque();
    float torque = myELM327.torque();
    uint16_t referenceTorque = myELM327.referenceTorque();

    /* Other */
    float batteryVoltage = myELM327.batteryVoltage();
    char vin[18];
    int8_t get_vin_blocking = myELM327.get_vin_blocking(vin);

    if (myELM327.nb_rx_state == ELM_SUCCESS) {
        #if DEBUG
        /* uint32_t supportedPIDs_1_20(); */
        SerialMon.print("OBD Engine Load: " + (String)engineLoad);
        SerialMon.print("OBD Engine Coolant Temp: " + (String)engineCoolantTemp);
        SerialMon.print("OBD Short Term Fuel TrimBank 1: " + (String)shortTermFuelTrimBank_1);
        SerialMon.print("OBD Long Term Fuel TrimBank 1: " + (String)longTermFuelTrimBank_1);
        SerialMon.print("OBD Short Term Fuel TrimBank 2: " + (String)shortTermFuelTrimBank_2);
        SerialMon.print("OBD Long Term Fuel TrimBank 2: " + (String)longTermFuelTrimBank_2);
        SerialMon.print("OBD Fuel Pressure: " + (String)fuelPressure);
        SerialMon.print("OBD Manifold Pressure: " + (String)manifoldPressure);
        SerialMon.print("OBD RPM: " + (String)rpm);
        SerialMon.print(" OBD KPH: " + (String)kph);
        SerialMon.print(" OBD MPH: " + (String)mph);
        SerialMon.print("OBD Timing Advance: " + (String)timingAdvance);
        SerialMon.print("OBD Intake Air Temp: " + (String)intakeAirTemp);
        SerialMon.print("OBD MAF Rate: " + (String)mafRate);
        SerialMon.print("OBD Throttle: " + (String)throttle);
        SerialMon.print("OBD Aux Input Status: " + (String)auxInputStatus);
        SerialMon.print("OBD Run Time: " + (String)runTime);

        /* uint32_t supportedPIDs_21_40(); */
        SerialMon.print("OBD Distance Travelled with MIL: " + (String)distTravelWithMIL);
        SerialMon.print("OBD Fuel Rail Pressure: " + (String)fuelRailPressure);
        SerialMon.print("OBD Fuel Rail Guage Pressure: " + (String)fuelRailGuagePressure);
        SerialMon.print("OBD Commanded EGR: " + (String)commandedEGR);
        SerialMon.print("OBD EGR Error: " + (String)egrError);
        SerialMon.print("OBD Commanded Evap Purge: " + (String)commandedEvapPurge);
        SerialMon.print("OBD Fuel Level: " + (String)fuelLevel);
        SerialMon.print("OBD Warm Ups Since Codes Cleared: " + (String)warmUpsSinceCodesCleared);
        SerialMon.print("OBD Distance Since Codes Cleared: " + (String)distSinceCodesCleared);
        SerialMon.print("OBD Evap System Vapor Pressure: " + (String)evapSysVapPressure);
        SerialMon.print("OBD Absolute Barometric Pressure: " + (String)absBaroPressure);
        SerialMon.print("OBD Catalyst Temperature Bank 1 Sensor 1: " + (String)catTempB1S1);
        SerialMon.print("OBD Catalyst Temperature Bank 2 Sensor 1: " + (String)catTempB2S1);
        SerialMon.print("OBD Catalyst Temperature Bank 1 Sensor 2: " + (String)catTempB1S2);
        SerialMon.print("OBD Catalyst Temperature Bank 2 Sensor 2: " + (String)catTempB2S2);

        /* uint32_t supportedPIDs_41_60(); */
        SerialMon.print("OBD Control Module Voltage: " + (String)ctrlModVoltage);
        SerialMon.print("OBD Absolute Load: " + (String)absLoad);
        SerialMon.print("OBD Commanded Air Fuel Ratio: " + (String)commandedAirFuelRatio);
        SerialMon.print("OBD Relative Throttle Position: " + (String)relativeThrottle);
        SerialMon.print("OBD Ambient Air Temperature: " + (String)ambientAirTemp);
        SerialMon.print("OBD Absolute Throttle Position B: " + (String)absThrottlePosB);
        SerialMon.print("OBD Absolute Throttle Position C: " + (String)absThrottlePosC);
        SerialMon.print("OBD Absolute Throttle Position D: " + (String)absThrottlePosD);
        SerialMon.print("OBD Absolute Throttle Position E: " + (String)absThrottlePosE);
        SerialMon.print("OBD Absolute Throttle Position F: " + (String)absThrottlePosF);
        SerialMon.print("OBD Commanded Throttle Actuator: " + (String)commandedThrottleActuator);
        SerialMon.print("OBD Time Run with MIL: " + (String)timeRunWithMIL);
        SerialMon.print("OBD Time Since Codes Cleared: " + (String)timeSinceCodesCleared);
        SerialMon.print("OBD Max MAF Rate: " + (String)maxMafRate);
        SerialMon.print("OBD Ethanol Percent: " + (String)ethonolPercent);
        SerialMon.print("OBD Absolute Evap System Vapor Pressure: " + (String)absEvapSysVapPressure);
        SerialMon.print("OBD Evap System Vapor Pressure: " + (String)evapSysVapPressure2);
        SerialMon.print("OBD Absolute Fuel Rail Pressure: " + (String)absFuelRailPressure);
        SerialMon.print("OBD Relative Pedal Position: " + (String)relativePedalPos);
        SerialMon.print("OBD Hybrid Battery Pack Remaining Life: " + (String)hybridBatLife);
        SerialMon.print("OBD Engine Oil Temperature: " + (String)oilTemp);
        SerialMon.print("OBD Fuel Injection Timing: " + (String)fuelInjectTiming);
        SerialMon.print("OBD Engine Fuel Rate: " + (String)fuelRate);

        /* uint32_t supportedPIDs_61_80(); */
        SerialMon.print("OBD Demanded Torque: " + (String)demandedTorque);
        SerialMon.print("OBD Actual Torque: " + (String)torque);
        SerialMon.print("OBD Reference Torque: " + (String)referenceTorque);

        /* Other */
        SerialMon.print("OBD Battery Voltage: " + (String)batteryVoltage);
        SerialMon.print("OBD VIN: " + (String)vin);
        SerialMon.print("OBD get_vin_blocking: " + (String)get_vin_blocking);
        #endif

        /* uint32_t supportedPIDs_1_20(); */
        packageAndSendMQTT((String)engineLoad, bluetoothobdmqttengineload);
        packageAndSendMQTT((String)engineCoolantTemp, bluetoothobdmqttenginecoolanttemp);
        packageAndSendMQTT((String)shortTermFuelTrimBank_1, bluetoothobdmqttshorttermfueltrimbank1);
        packageAndSendMQTT((String)longTermFuelTrimBank_1, bluetoothobdmqttlongtermfueltrimbank1);
        packageAndSendMQTT((String)shortTermFuelTrimBank_2, bluetoothobdmqttshorttermfueltrimbank2);
        packageAndSendMQTT((String)longTermFuelTrimBank_2, bluetoothobdmqttlongtermfueltrimbank2);
        packageAndSendMQTT((String)fuelPressure, bluetoothobdmqttfuelpressure);
        packageAndSendMQTT((String)manifoldPressure, bluetoothobdmqttmanifoldpressure);
        packageAndSendMQTT((String)rpm, bluetoothobdmqttrpm);
        packageAndSendMQTT((String)kph, bluetoothobdmqttkph);
        packageAndSendMQTT((String)mph, bluetoothobdmqttmph);
        packageAndSendMQTT((String)timingAdvance, bluetoothobdmqtttimingadvance);
        packageAndSendMQTT((String)intakeAirTemp, bluetoothobdmqttintakeairtemp);
        packageAndSendMQTT((String)mafRate, bluetoothobdmqttmafrate);
        packageAndSendMQTT((String)throttle, bluetoothobdmqttthrottle);
        packageAndSendMQTT((String)auxInputStatus, bluetoothobdmqttauxinputstatus);
        packageAndSendMQTT((String)runTime, bluetoothobdmqttruntime);

        /* uint32_t supportedPIDs_21_40(); */
        packageAndSendMQTT((String)distTravelWithMIL, bluetoothobdmqttdisttravelwithmil);
        packageAndSendMQTT((String)fuelRailPressure, bluetoothobdmqttfuelrailpressure);
        packageAndSendMQTT((String)fuelRailGuagePressure, bluetoothobdmqttfuelrailguagepressure);
        packageAndSendMQTT((String)commandedEGR, bluetoothobdmqttcommandedegr);
        packageAndSendMQTT((String)egrError, bluetoothobdmqttegrerror);
        packageAndSendMQTT((String)commandedEvapPurge, bluetoothobdmqttcommandedevappurge);
        packageAndSendMQTT((String)fuelLevel, bluetoothobdmqttfuellevel);
        packageAndSendMQTT((String)warmUpsSinceCodesCleared, bluetoothobdmqttwarmupssincecodescleared);
        packageAndSendMQTT((String)distSinceCodesCleared, bluetoothobdmqttdistsincecodescleared);
        packageAndSendMQTT((String)evapSysVapPressure, bluetoothobdmqttevapsysvappressure);
        packageAndSendMQTT((String)absBaroPressure, bluetoothobdmqttabsbaropressure);
        packageAndSendMQTT((String)catTempB1S1, bluetoothobdmqttcattempb1s1);
        packageAndSendMQTT((String)catTempB2S1, bluetoothobdmqttcattempb2s1);
        packageAndSendMQTT((String)catTempB1S2, bluetoothobdmqttcattempb1s2);
        packageAndSendMQTT((String)catTempB2S2, bluetoothobdmqttcattempb2s2);

        /* uint32_t supportedPIDs_41_60(); */
        packageAndSendMQTT((String)ctrlModVoltage, bluetoothobdmqttctrlmodvoltage);
        packageAndSendMQTT((String)absLoad, bluetoothobdmqttabsload);
        packageAndSendMQTT((String)commandedAirFuelRatio, bluetoothobdmqttcommandedairfuelratio);
        packageAndSendMQTT((String)relativeThrottle, bluetoothobdmqttrelativethrottle);
        packageAndSendMQTT((String)ambientAirTemp, bluetoothobdmqttambientairtemp);
        packageAndSendMQTT((String)absThrottlePosB, bluetoothobdmqttabsthrottleposb);
        packageAndSendMQTT((String)absThrottlePosC, bluetoothobdmqttabsthrottleposc);
        packageAndSendMQTT((String)absThrottlePosD, bluetoothobdmqttabsthrottleposd);
        packageAndSendMQTT((String)absThrottlePosE, bluetoothobdmqttabsthrottlepose);
        packageAndSendMQTT((String)absThrottlePosF, bluetoothobdmqttabsthrottleposf);
        packageAndSendMQTT((String)commandedThrottleActuator, bluetoothobdmqttcommandedthrottleactuator);
        packageAndSendMQTT((String)timeRunWithMIL, bluetoothobdmqtttimerunwithmil);
        packageAndSendMQTT((String)timeSinceCodesCleared, bluetoothobdmqtttimesincecodescleared);
        packageAndSendMQTT((String)maxMafRate, bluetoothobdmqttmaxmafrate);
        packageAndSendMQTT((String)ethonolPercent, bluetoothobdmqttethonolpercent);
        packageAndSendMQTT((String)absEvapSysVapPressure, bluetoothobdmqttabsevapsysvappressure);
        packageAndSendMQTT((String)evapSysVapPressure2, bluetoothobdmqttevapsysvappressure2);
        packageAndSendMQTT((String)absFuelRailPressure, bluetoothobdmqttabsfuelrailpressure);
        packageAndSendMQTT((String)relativePedalPos, bluetoothobdmqttrelativepedalpos);
        packageAndSendMQTT((String)hybridBatLife, bluetoothobdmqtthybridbatlife);
        packageAndSendMQTT((String)oilTemp, bluetoothobdmqttoiltemp);
        packageAndSendMQTT((String)fuelInjectTiming, bluetoothobdmqttfuelinjecttiming);
        packageAndSendMQTT((String)fuelRate, bluetoothobdmqttfuelrate);

        /* uint32_t supportedPIDs_61_80(); */
        packageAndSendMQTT((String)demandedTorque, bluetoothobdmqttdemandedtorque);
        packageAndSendMQTT((String)torque, bluetoothobdmqtttorque);
        packageAndSendMQTT((String)referenceTorque, bluetoothobdmqttreferencetorque);

        /* Other */
        packageAndSendMQTT((String)batteryVoltage, bluetoothobdmqttbatteryvoltage);
        packageAndSendMQTT((String)get_vin_blocking, bluetoothobdmqttvin);
    } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
        #if DEBUG
        myELM327.printError();
        #endif
        packageAndSendMQTT((String)myELM327.payload, bluetoothobdmqttobdpayload);

        String state = "";
        if (myELM327.nb_rx_state == ELM_SUCCESS) {
            state = "ELM_SUCCESS";
        } else if (myELM327.nb_rx_state == ELM_NO_RESPONSE) {
            state = "ELM_NO_RESPONSE";
        } else if (myELM327.nb_rx_state == ELM_BUFFER_OVERFLOW) {
            state = "ELM_BUFFER_OVERFLOW";
        } else if (myELM327.nb_rx_state == ELM_UNABLE_TO_CONNECT) {
            state = "ELM_UNABLE_TO_CONNECT";
        } else if (myELM327.nb_rx_state == ELM_NO_DATA) {
            state = "ELM_NO_DATA";
        } else if (myELM327.nb_rx_state == ELM_STOPPED) {
            state = "ELM_STOPPED";
        } else if (myELM327.nb_rx_state == ELM_TIMEOUT) {
            state = "ELM_TIMEOUT";
        } else if (myELM327.nb_rx_state == ELM_BUFFER_OVERFLOW) {
            state = "BUFFER OVERFLOW";
        } else if (myELM327.nb_rx_state == ELM_GENERAL_ERROR) {
            state = "ELM_GENERAL_ERROR";
        } else {
            state = "No error detected";
        }

        packageAndSendMQTT(state, bluetoothobdmqttobddebug);
    }
}