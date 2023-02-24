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
- Detailed user configuration (accessible by connecting to the ESP's hotspot and going to http://192.168.4.1)
- Cellular connectivity
- Automatically update the ESP's clock
- MQTT connection
- Report battery status (percentage, voltage, USB charging, solar charging & discharging)
- Report network details
- Report location status (latitude, longitude, speed, altitude, visible satellites, accuracy) with cellular fallback
- Report the uptime of the ESP in seconds
- Report your car's temperature, humidity and air pressure using a BME280 sensor
- Report the light level inside your car using a TSL2561 sensor
- Detect motion inside your car's cabin using a PIR sensor
- OTA updates through a secure hotspot
- Advanced config file (`config.h`)

## Software Requirements
- IDE of choise with PlatformIO (VSCode is recommended)
- USB <-> Serial driver ([`macOS`](https://github.com/Xinyuan-LilyGO/CH9102_Mac_Driver) | [`Linux`](https://github.com/gorgiaxx/CH34x-Driver-Linux) | [`Windows`](https://github.com/Xinyuan-LilyGO/CH9102_Driver))
- MQTT Broker

## Hardware Requirements
- LilyGO® TTGO T-SIM7600E-L1C ESP32 (Other ESP32 TTGO models could also work)
- A Nano SIM Card with an active internet plan
- Cellular antenna
- GPS antenna
- 18650B battery
- BME280 sensor (optional)
- TSL2561 sensor (optional)
- PIR sensor (like the AM312) thats pulled low by default (optional)

## Configuration
### Cellular
- `APN` - The APN URL of your cellular provider
- `Username` - The APN username of your cellular provider
- `Password` - The APN password of your cellular provider
- `SIM PIN` - The PIN of your SIM card (leave blank if you don't have one)
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
- `I2C Address` - The I2C address of the TSL2561 sensor (0x29 or 0x39)
- `Gain` - The gain of the TSL2561 sensor (auto, 1x or 16x)
- `MQTT` - The MQTT topic to publish the light level to
### PIR
- `Enable` - Enable or disable the PIR sensor
- `GPIO` - The GPIO pin the PIR sensor is connected to
- `MQTT` - The MQTT topic to publish the motion status to
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
### Misc MQTT topics
- `Uptime` - The MQTT topic to publish the uptime to
- `Version` - The MQTT topic to publish the version to

## Credits
[Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO/T-SIM7600X) - Basic T-SIM7600X implementation
