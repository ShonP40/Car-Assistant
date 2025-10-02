<p align="center">
    <a href="https://shon.codes"><img src="assets/logo/Car%20Assistant.svg" width="86" height="86"></a>
</p>

<h1 align="center">Car Assistant (Experimental ESPHome version)</h1>
<h3 align="center">Keep track of your car remotely through Home Assistant</h3>

## Sponsor
![image](https://user-images.githubusercontent.com/13995143/184159660-3a476f93-58ee-4487-b305-6b044c7b1c4e.png)

This project has been sponsored by [PCBWay](https://www.pcbway.com/)!

You can order this project from PCBWay at a $5 discount by using my [referal link](https://www.pcbway.com/setinvite.aspx?inviteid=590728)

## Features
- OTA updates
- Cellular connectivity
- Automatically update the ESP's clock
- VPN connection
- SD card support through a web server
- Broadcast a Bluetooth iBeacon (to detect which phones are in the car, by using the `iBeacon Scanner` sensor in the Home Assistant app)
- Report battery status (percentage, voltage, USB charging, solar charging & discharging)
- Report network details
- Report location status (latitude, longitude, speed, altitude, visible satellites, accuracy) with cellular fallback
- Report the uptime of the ESP in seconds
- Report your car's temperature, humidity and air pressure using a BME280 sensor
- Report the light level inside your car using a TSL2561 sensor
- Detect motion inside your car's cabin using a PIR sensor
- Report the CPU temperature and frequency
- Report the free RAM amount
- Reboot the ESP32 & Modem remotely
- Broadcast a WiFi hotspot with internet access & ESPHome Web UI

## Software Requirements
- ESPHome (2025.7.0 or later)
- USB <-> Serial driver ([`macOS`](https://github.com/Xinyuan-LilyGO/CH9102_Mac_Driver) | [`Linux`](https://github.com/gorgiaxx/CH34x-Driver-Linux) | [`Windows`](https://github.com/Xinyuan-LilyGO/CH9102_Driver))
- Home Assistant server (An example config can be found [here](home-assistant.yaml))
- WireGuard VPN server

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

## Connectors
- I2C1 - GPIO 21 & 22
- I2C2 - GPIO 21 & 22
- GPIO - GPIO 19 & 23

## Boards preview
![Assembled PCB](./PCB/Photos/PCB/13%20-%20Assembled%20(All).jpeg)

## Enclosures preview
![Assembled Enclosures](./PCB/Photos/Enclosure/12%20-%20Assembled%20(All).jpeg)

More pictures can be found [here](./PCB/Photos)