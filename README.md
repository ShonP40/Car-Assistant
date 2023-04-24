<p align="center">
    <a href="https://shon.codes"><img src="assets/logo/Car%20Assistant.svg" width="86" height="86"></a>
</p>

<h1 align="center">Car Assistant</h1>
<h3 align="center">Keep track of your car remotely through Home Assistant</h3>

## Sponsor
![image](https://user-images.githubusercontent.com/13995143/184159660-3a476f93-58ee-4487-b305-6b044c7b1c4e.png)

This project has been sponsored by [PCBWay](https://www.pcbway.com/)!

You can order this project from PCBWay at a $5 discount by using my [referal link](https://www.pcbway.com/setinvite.aspx?inviteid=590728)

## Features
- Detailed user [configuration](https://github.com/ShonP40/Car-Assistant#configuration-web-ui) (accessible by connecting to the ESP's hotspot and going to http://192.168.4.1)
- Advanced build [configuration](https://github.com/ShonP40/Car-Assistant#advanced-build-configuration-platformioini) with verbose support (`platformio.ini`)
- OTA updates through a secure hotspot
- Cellular connectivity
- Automatically update the ESP's clock
- MQTT connection
- SD card support through an FTP server (set your connection mode to `Active (PORT)`)
- Dynamic CPU frequency scaling
- Report battery status (percentage, voltage, USB charging, solar charging & discharging)
- Report network details
- Report location status (latitude, longitude, speed, altitude, visible satellites, accuracy) with cellular fallback
- Report the uptime of the ESP in seconds
- Report your car's temperature, humidity and air pressure using a BME280 sensor
- Report the light level inside your car using a TSL2561 sensor
- Detect motion inside your car's cabin using a PIR sensor
- Report the CPU temperature and frequency
- Report the free RAM amount
- Report the ESP32's hall sensor value
- Bluetooth OBD2 adapters support (incomplete)

## Software Requirements
- IDE of choise with PlatformIO (VSCode is recommended)
- USB <-> Serial driver ([`macOS`](https://github.com/Xinyuan-LilyGO/CH9102_Mac_Driver) | [`Linux`](https://github.com/gorgiaxx/CH34x-Driver-Linux) | [`Windows`](https://github.com/Xinyuan-LilyGO/CH9102_Driver))
- MQTT Broker
- Home Assistant server (optional. An example config can be found [here](home-assistant.yaml))

## Hardware Requirements
- LilyGO® TTGO T-SIM7600E-L1C ESP32 (Other ESP32 TTGO models could also work)
- A Nano SIM Card with an active internet plan
- Cellular antenna
- GPS antenna
- 18650B battery
- BME280 sensor (optional)
- TSL2561 sensor (optional)
- PIR sensor (like the AM312) thats pulled low by default (optional)
- Micro SD Card formatted as FAT32 (optional)
- Bluetooth (not BLE) OBD2 adapter (like the Vgate iCar Pro) (optional)

## Connectors
- I2C1 - GPIO 21 & 22
- I2C2 - GPIO 21 & 22
- GPIO - GPIO 19 & 23

## Boards preview
![Assembled PCB](./PCB/Photos/PCB/13%20-%20Assembled%20(All).jpeg)

## Enclosures preview
![Assembled Enclosures](./PCB/Photos/Enclosure/12%20-%20Assembled%20(All).jpeg)

More pictures can be found [here](./PCB/Photos)

## Configuration (Web UI)
### Cellular
- `APN` - The APN URL of your cellular provider
- `Username` - The APN username of your cellular provider
- `Password` - The APN password of your cellular provider
- `SIM PIN` - The PIN of your SIM card (leave blank if you don't have one)
### Clock
- `Timezone` - The UTC offset of your timezone (in seconds, e.g. UTC+2 = 7200)
- `Daylight Saving Time` - Enable or disable daylight saving time (will add 1 hour to the UTC offset)
### WiFi AP
- `SSID` - The SSID of the WiFi AP that the ESP will create
- `Password` - The password of the WiFi AP that the ESP will create
### MQTT Broker
- `Address` - The IP address / domain name of your MQTT broker
- `Port` - The port of your MQTT broker
- `Client name` - The name you want to give to the ESP on your MQTT broker
- `Username` - The username of your MQTT broker
- `Password` - The password of your MQTT broker
### Location
- `GNSS Mode` - Constellations the modem should use (0 = GPS + GLONASS, 1 = GPS + BEIDOU, 2 = GPS + GALILEO, 3 = GPS + QZSS)
- `DPO` - Direct Platform Orientation (0 = Disable, 1 = Enable)
### Sensors
- `Enable` - Enable or disable all the sensors
### BME280
- `Enable` - Enable or disable the BME280 sensor
- `I2C Address` - The I2C address of the BME280 sensor (0x76 or 0x77)
- `MQTT Temp` - The MQTT topic to publish the temperature to
- `MQTT Pressure` - The MQTT topic to publish the pressure to
- `MQTT Humidity` - The MQTT topic to publish the humidity to
### TSL2561
- `Enable` - Enable or disable the TSL2561 sensor
- `I2C Address` - The I2C address of the TSL2561 sensor (0x29, 0x39 or 0x49)
- `Gain` - The gain of the TSL2561 sensor (auto, 1x or 16x)
- `MQTT` - The MQTT topic to publish the light level to
### PIR
- `Enable` - Enable or disable the PIR sensor
- `GPIO` - The GPIO pin the PIR sensor is connected to
- `MQTT` - The MQTT topic to publish the motion status to
### CPU Temperature
- `Enable` - Enable or disable reporting the CPU temperature sensor value
- `MQTT` - The MQTT topic to publish the CPU temperature to
### CPU Frequency
- `Enable` - Enable or disable reporting the CPU frequency value
- `MQTT` - The MQTT topic to publish the CPU frequency to
### Free RAM
- `Enable` - Enable or disable reporting the free RAM value
- `MQTT` - The MQTT topic to publish the free RAM value to
### Hall
- `Enable` - Enable or disable reporting the hall sensor value
- `MQTT` - The MQTT topic to publish the hall sensor value to
### Modem MQTT topics
- `Info` - The MQTT topic to publish the modem info to
- `CCID` - The MQTT topic to publish the modem CCID to
- `IMSI` - The MQTT topic to publish the modem IMSI to
- `Operator` - The MQTT topic to publish the modem operator to
- `Signal Quality` - The MQTT topic to publish the modem signal quality to
- `Public IP` - The MQTT topic to publish the modem public IP to
### Battery MQTT topics
- `Percentage` - The MQTT topic to publish the battery percentage to
- `Voltage` - The MQTT topic to publish the battery voltage to
- `Status` - The MQTT topic to publish the battery charging status to (charging, USB charging or discharging)
### Location MQTT topics
- `Type` - The MQTT topic to publish the location type to (GPS or cellular)
- `Latitude` - The MQTT topic to publish the location latitude to
- `Longitude` - The MQTT topic to publish the location longitude to
- `Speed` - The MQTT topic to publish the location speed to
- `Altitude` - The MQTT topic to publish the location altitude to
- `Accuracy` - The MQTT topic to publish the location accuracy to
### FTP
- `Enable` - Enable or disable the FTP server
- `Username` - The username of the FTP server
- `Password` - The password of the FTP server
### Bluetooth
- `Enable` - Enable or disable Bluetooth
- `Name` - The Bluetooth name the ESP will advertise
- `OBD Identifier` - The name or MAC address of the OBD2 adapter
- `OBD Payload MQTT` - The MQTT topic to publish the OBD2 error payload to
- `OBD Debug MQTT` - The MQTT topic to publish the OBD2 debug errors to
- `Engine Load MQTT` - The MQTT topic to publish the engine load to
- `Engine Coolant Temp MQTT` - The MQTT topic to publish the engine coolant temperature to
- `Short Term Fuel Trim Bank 1 MQTT` - The MQTT topic to publish the short term fuel trim bank 1 to
- `Long Term Fuel Trim Bank 1 MQTT` - The MQTT topic to publish the long term fuel trim bank 1 to
- `Short Term Fuel Trim Bank 2 MQTT` - The MQTT topic to publish the short term fuel trim bank 2 to
- `Long Term Fuel Trim Bank 2 MQTT` - The MQTT topic to publish the long term fuel trim bank 2 to
- `Fuel Pressure MQTT` - The MQTT topic to publish the fuel pressure to
- `Manifold Pressure MQTT` - The MQTT topic to publish the manifold pressure to
- `RPM MQTT` - The MQTT topic to publish the engine RPM to
- `KPH MQTT` - The MQTT topic to publish the speed in KPH to
- `MPH MQTT` - The MQTT topic to publish the speed in MPH to
- `Timing Advance MQTT` - The MQTT topic to publish the timing advance to
- `Intake Air Temp MQTT` - The MQTT topic to publish the intake air temperature to
- `MAF Rate MQTT` - The MQTT topic to publish the MAF rate to
- `Throttle MQTT` - The MQTT topic to publish the throttle position to
- `Aux Input Status MQTT` - The MQTT topic to publish the aux input status to
- `Run Time MQTT` - The MQTT topic to publish the run time to
- `Dist Travel With Mil MQTT` - The MQTT topic to publish the distance traveled with MIL to
- `Fuel Rail Pressure MQTT` - The MQTT topic to publish the fuel rail pressure to
- `Fuel Rail Guage Pressure MQTT` - The MQTT topic to publish the fuel rail gauge pressure to
- `Commanded EGR MQTT` - The MQTT topic to publish the commanded EGR to
- `EGR Error MQTT` - The MQTT topic to publish the EGR error to
- `Commanded Evap Purge MQTT` - The MQTT topic to publish the commanded evap purge to
- `Fuel Level MQTT` - The MQTT topic to publish the fuel level to
- `Warm Ups Since Codes Cleared MQTT` - The MQTT topic to publish the warm ups since codes cleared to
- `Dist Since Codes Cleared MQTT` - The MQTT topic to publish the distance since codes cleared to
- `Evap Sys Vap Pressure MQTT` - The MQTT topic to publish the evap system vapor pressure to
- `ABS Baro Pressure MQTT` - The MQTT topic to publish the ABS barometric pressure to
- `Cat Temp B1 S1 MQTT` - The MQTT topic to publish the catalyst temperature bank 1 sensor 1 to
- `Cat Temp B2 S1 MQTT` - The MQTT topic to publish the catalyst temperature bank 2 sensor 1 to
- `Cat Temp B1 S2 MQTT` - The MQTT topic to publish the catalyst temperature bank 1 sensor 2 to
- `Cat Temp B2 S2 MQTT` - The MQTT topic to publish the catalyst temperature bank 2 sensor 2 to
- `Ctrl Mod Voltage MQTT` - The MQTT topic to publish the control module voltage to
- `ABS Load MQTT` - The MQTT topic to publish the ABS load to
- `Commanded Air Fuel Ratio MQTT` - The MQTT topic to publish the commanded air fuel ratio to
- `Relative Throttle MQTT` - The MQTT topic to publish the relative throttle position to
- `Ambient Air Temp MQTT` - The MQTT topic to publish the ambient air temperature to
- `ABS Throttle Pos B MQTT` - The MQTT topic to publish the ABS throttle position B to
- `ABS Throttle Pos C MQTT` - The MQTT topic to publish the ABS throttle position C to
- `Accel Pedal Pos D MQTT` - The MQTT topic to publish the accelerator pedal position D to
- `Accel Pedal Pos E MQTT` - The MQTT topic to publish the accelerator pedal position E to
- `Accel Pedal Pos F MQTT` - The MQTT topic to publish the accelerator pedal position F to
- `Commanded Throttle Actuator MQTT` - The MQTT topic to publish the commanded throttle actuator to
- `Time Run With MIL MQTT` - The MQTT topic to publish the time run with MIL to
- `Time Since Codes Cleared MQTT` - The MQTT topic to publish the time since codes cleared to
- `Max MAF Rate MQTT` - The MQTT topic to publish the max MAF rate to
- `Ethonol Percent MQTT` - The MQTT topic to publish the ethonol percent to
- `ABS Evap Sys Vap Pressure MQTT` - The MQTT topic to publish the ABS evap system vapor pressure to
- `Evap Sys Vap Pressure 2 MQTT ` - The MQTT topic to publish the evap system vapor pressure 2 to
- `ABS Fuel Rail Pressure MQTT` - The MQTT topic to publish the ABS fuel rail pressure to
- `Eelative Pedal Pos MQTT` - The MQTT topic to publish the relative pedal position to
- `Hybrid Bat Life MQTT` - The MQTT topic to publish the hybrid battery life to
- `Oil Temp MQTT` - The MQTT topic to publish the oil temperature to
- `Fuel Inject Timing MQTT` - The MQTT topic to publish the fuel injection timing to
- `Fuel Rate MQTT` - The MQTT topic to publish the fuel rate to
- `Demanded Torque MQTT` - The MQTT topic to publish the demanded torque to
- `Torque MQTT` - The MQTT topic to publish the torque to
- `Reference Torque MQTT` - The MQTT topic to publish the reference torque to
- `Battery Voltage MQTT` - The MQTT topic to publish the battery voltage to
- `VIN MQTT` - The MQTT topic to publish the VIN of your car to
### Misc MQTT topics
- `Uptime` - The MQTT topic to publish the uptime to
- `Version` - The MQTT topic to publish the version number to
### Power Saving
- `Dynamic Frequency` - Lower the CPU frequency to 160MHz when running on battery
- `Low Power mode on battery` - Lower the CPU frequency to 80MHz (instead of 160MHz) and turn GPS off when running on battery (only works if Dynamic Frequency is enabled)

## Advanced Build Configuration (`platformio.ini`)
### Debug
- `DEBUG` - Enable or disable debug messages
### Modem config
- `UART_BAUD` - The baud rate of the serial port & modem
- `MODEM_TX` - The TX pin of the modem
- `MODEM_RX` - The RX pin of the modem
- `MODEM_PWRKEY` - The PWRKEY pin of the modem
- `MODEM_DTR` - The DTR pin of the modem
- `MODEM_RI` - The RI pin of the modem
- `MODEM_FLIGHT` - The flight mode pin of the modem
- `MODEM_STATUS` - The status pin of the modem
### Status LED
- `LED_PIN` - The pin of the status LED
### Indicator pins
- `USB_INDICATOR` - Detect if the board is being powered by USB
- `SOLAR_INDICATOR` - Detect if the board is being powered by solar
### Bluetooth
- `PAIR_MAX_DEVICES` - The maximum number of devices the ESP will store before resetting
### Modem model
- `TINY_GSM_MODEM_<MODEL>` - The model of the modem (only SIM7600 has been tested)

## Credits
- [Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO/T-SIM7600X) - Basic T-SIM7600X implementation
- [Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-wi-fi-manager-asyncwebserver/) - Base webserver code
- [vshymanskyy](https://github.com/vshymanskyy) - [TinyGSM](https://github.com/vshymanskyy/TinyGSM) & [StreamDebugger](https://github.com/vshymanskyy/StreamDebugger) libraries
- [knolleary](https://github.com/knolleary) - [PubSubClient](https://github.com/knolleary/pubsubclient) library
- [adafruit](https://github.com/adafruit) - [Unified Sensor](https://github.com/adafruit/Adafruit_Sensor), [BME280](https://github.com/adafruit/Adafruit_BME280_Library) & [TSL2561](https://github.com/adafruit/Adafruit_TSL2561) libraries
- [ayushsharma82](https://github.com/ayushsharma82) - [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA) library
- [me-no-dev](https://github.com/me-no-dev) - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) & [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) libraries
- [bblanchon](https://github.com/bblanchon) - [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library
- [peterus](https://github.com/peterus) - [ESP-FTP-Server](https://github.com/peterus/ESP-FTP-Server-Lib) library
- [PowerBroker2](https://github.com/PowerBroker2) - [ELMduino](https://github.com/PowerBroker2/ELMduino) library
