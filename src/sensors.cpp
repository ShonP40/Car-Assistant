#include "sensors.h"

uint8_t i2cAddress(String type, String value) {
  if (type == "BME280") {
    if (value == "0x76") {
        return 0x76;
    } else if (value == "0x77") {
        return 0x77;
    } else {
        return 0;
    }
  } else if (type == "TSL2561") {
    if (value == "0x39") {
        return 0x39;
    } else if (value == "0x29") {
        return 0x29;
    } else if (value == "0x49") {
        return 0x49;
    } else {
        return 0;
    }
  } else {
    return 0;
  }
}

// BME280
Adafruit_BME280 bme; // I2C
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void initSensors() {
  if (sensorsenable == "true") {
    if (sensorsenablebme280 == "true") {
      // Initialize the BME280 sensor
      if (!bme.begin(i2cAddress("BME280", sensorbme280i2caddress))) {
        #if DEBUG
        SerialMon.println("Could not find a valid BME280 sensor, check wiring!");
        #endif
      }
    }

    if (sensorsenablepir == "true") {
      // Initialize the PIR sensor
      pinMode(stringToInt(sensorpirpin), INPUT);
    }
  }
}

void readSensors() {
  if (sensorsenable == "true") {
    // BME 280
    if (sensorsenablebme280 == "true") {
      sensors_event_t temp_event, pressure_event, humidity_event;
      bme_temp->getEvent(&temp_event);
      bme_pressure->getEvent(&pressure_event);
      bme_humidity->getEvent(&humidity_event);

      #if DEBUG
      SerialMon.print("Temperature = ");
      SerialMon.print(temp_event.temperature);
      SerialMon.println(" *C");

      SerialMon.print("Pressure = ");
      SerialMon.print(pressure_event.pressure);
      SerialMon.println(" hPa");

      SerialMon.print("Humidity = ");
      SerialMon.print(humidity_event.relative_humidity);
      SerialMon.println(" %");
      #endif

      packageAndSendMQTT(String(temp_event.temperature), mqttsensorsbme280temperature);
      packageAndSendMQTT(String(pressure_event.pressure), mqttsensorsbme280pressure);
      packageAndSendMQTT(String(humidity_event.relative_humidity), mqttsensorsbme280humidity);
    }

    // TSL2561
    Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(i2cAddress("TSL2561", sensortsl2561i2caddress), 12345);

    if (sensorsenabletsl2561 == "true") {
      // Initialize the TSL2561 sensor
      if (!tsl.begin()) {
        #if DEBUG
        SerialMon.println("Could not find a valid TSL2561 sensor, check wiring!");
        #endif
      }
    
      // Set the gain
      if (sensortsl2561gain == "auto") {
        tsl.enableAutoRange(true);
      } else if (sensortsl2561gain == "1x") {
        tsl.setGain(TSL2561_GAIN_1X);
      } else if (sensortsl2561gain == "16x") {
        tsl.setGain(TSL2561_GAIN_16X);
      } else {
        #if DEBUG
        SerialMon.println("Invalid TSL2561_GAIN value, please use \"auto\", \"1x\" or \"16x\"");
        #endif
      }
      
      sensors_event_t light_event;
      tsl.getEvent(&light_event);

      #if DEBUG
      SerialMon.print("Light = ");
      SerialMon.print(light_event.light);
      SerialMon.println(" lux");
      #endif

      packageAndSendMQTT(String(light_event.light), mqttsensorstsl2561lux);
    }

    // PIR
    if (sensorsenablepir == "true") {
      int pirState = digitalRead(stringToInt(sensorpirpin));

      #if DEBUG
      SerialMon.print("PIR = ");
      SerialMon.println(pirState);
      #endif

      packageAndSendMQTT(String(pirState), mqttsensorspir);
    }
  }
}