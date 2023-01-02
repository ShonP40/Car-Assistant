#include "sensors.h"

#if BME280_ENABLED
// BME280
Adafruit_BME280 bme; // I2C
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
#endif

#if TSL2561_ENABLED
// TSL2561
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_I2C_ADDRESS, 12345);
#endif

void initSensors() {
  #if BME280_ENABLED && DEBUG
  // Initialize the BME280 sensor
  if (!bme.begin(BME280_I2C_ADDRESS)) {
    SerialMon.println("Could not find a valid BME280 sensor, check wiring!");
  }
  #endif

  #if TSL2561_ENABLED
  // Initialize the TSL2561 sensor
  #if DEBUG
  if(!tsl.begin()) {
    SerialMon.println("Could not find a valid TSL2561 sensor, check wiring!");
  }
  #endif
  
  // Set the gain
  if (TSL2561_GAIN == "auto") {
    tsl.enableAutoRange(true);
  } else if (TSL2561_GAIN == "1x") {
    tsl.setGain(TSL2561_GAIN_1X);
  } else if (TSL2561_GAIN == "16x") {
    tsl.setGain(TSL2561_GAIN_16X);
  } else {
    SerialMon.println("Invalid TSL2561_GAIN value, please use \"auto\", \"1x\" or \"16x\"");
  }
  #endif

  #if PIR_ENABLED
  // Initialize the PIR sensor
  pinMode(PIR_PIN, INPUT);
  #endif
}

void readSensors() {
  #if BME280_ENABLED
  // BME 280
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

  packageAndSendMQTT(String(temp_event.temperature), MQTT_BME280_TEMPERATURE);
  packageAndSendMQTT(String(pressure_event.pressure), MQTT_BME280_PRESSURE);
  packageAndSendMQTT(String(humidity_event.relative_humidity), MQTT_BME280_HUMIDITY);
  #endif

  #if TSL2561_ENABLED
  // TSL2561
  sensors_event_t light_event;
  tsl.getEvent(&light_event);

  #if DEBUG
  SerialMon.print("Light = ");
  SerialMon.print(light_event.light);
  SerialMon.println(" lux");
  #endif

  packageAndSendMQTT(String(light_event.light), MQTT_TSL2561_LUX);
  #endif

  #if PIR_ENABLED
  // PIR
  int pirState = digitalRead(PIR_PIN);

  #if DEBUG
  SerialMon.print("PIR = ");
  SerialMon.println(pirState);
  #endif

  packageAndSendMQTT(String(pirState), MQTT_PIR);
  #endif
}