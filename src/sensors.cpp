#include "sensors.h"

#if BME280_ENABLED
// BME280
Adafruit_BME280 bme; // I2C
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
#endif

void initSensors() {
  #if BME280_ENABLED
  // Initialize the BME280 sensor
  if (!bme.begin(BME280_I2C_ADDRESS)) {
    SerialMon.println("Could not find a valid BME280 sensor, check wiring!");
  }
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
}