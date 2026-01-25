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
- OTA updates (Over a WireGuard VPN connection, cellular NAT IP or locally through the WiFi hotspot)
- Cellular connectivity
- Automatically update the ESP's clock
- WireGuard VPN connection
- SD card support through a web server
- Broadcast a Bluetooth iBeacon (to detect which phones are in the car, by using the `iBeacon Scanner` sensor in the Home Assistant app)
- Report battery status (percentage, voltage, USB charging, solar charging & discharging)
- Report network details (public IP, Cellular NAT IP, WireGuard IP, Cellular network type, Cellular signal strength and Cellular network type)
- Report location status (latitude, longitude, speed, altitude, visible satellites, horizontal dilution of precision, accuracy and course)
- Report the uptime of the ESP in seconds
- Report your car's temperature, humidity and air pressure using a BME280 sensor
- Report the light level inside your car using a TSL2561 sensor
- Detect motion inside your car's cabin using a PIR sensor or a mmWave radar
- Report the CPU temperature and frequency
- Report the free RAM & PSRAM amounts
- Reboot the ESP32 & Modem remotely
- Broadcast a WiFi hotspot with internet access

## Software Requirements
- ESPHome (2025.8.0 or later)
- Home Assistant server
- WireGuard VPN server or an MQTT broker
- USB <-> Serial driver ([`macOS`](https://github.com/Xinyuan-LilyGO/CH9102_Mac_Driver) | [`Linux`](https://github.com/gorgiaxx/CH34x-Driver-Linux) | [`Windows`](https://github.com/Xinyuan-LilyGO/CH9102_Driver))

## Hardware Requirements
- LilyGO® TTGO T-SIM7600E-L1C ESP32 (Other ESP32 TTGO models could also work)
- A Nano SIM Card with an active internet plan
- Cellular antenna
- GPS antenna
- 18650B battery
- BME280 sensor (optional)
- TSL2561 sensor (optional)
- PIR sensor (like the AM312) thats pulled low by default (optional)
- LD2410S mmWave radar (optional)
- Micro SD Card formatted as FAT32 (optional)

## Connectors
- I2C1 - GPIO 21 (SDA) & 22 (SCL)
- I2C2 - GPIO 21 (SDA) & 22 (SCL)
- GPIO - GPIO 19 & 23

## Boards preview
![Assembled PCB](./PCB/Photos/PCB/14%20-%20Assembled%20(All).jpeg)

## Enclosures preview
![Assembled Enclosures](./PCB/Photos/Enclosure/12%20-%20Assembled%20(All).jpeg)

More pictures can be found [here](./PCB/Photos)

## Installation

Follow these steps to get your Car Assistant device up and running with ESPHome.

### ESPHome Integration Setup

1. Install the ESPHome add-on in Home Assistant if not already installed:
   - Go to `Settings` → `Add-ons` → `Add-on Store`
   - Search for `ESPHome`
   - Click `Install`
2. Start the ESPHome add-on and verify that it's running
3. Access the ESPHome dashboard through Home Assistant

### Setup Device

1. In the ESPHome dashboard, click `+ New Device`
2. Name your device (e.g., "Car Assistant")
3. Select `ESP32` as your device type
4. Copy the default configuration to your new device from [here](./car-assistant.yaml)
5. Modify the configuration in the user config section
6. Click `Save` and then `Install`

### Device Flashing

Initial flashing must be done via serial connection.

[ESPHome Web](https://web.esphome.io) is recommended for the simplest experience.

#### Required Hardware
- USB-C to USB-A cable

#### Flashing Process
1. Connect the T-SIM7600E-L1C to a computer using a USB-C cable
2. Visit [ESPHome Web](https://web.esphome.io)
3. Connect to your device and flash the firmware you compiled earlier
4. After a successful flash, the device will restart and be ready for OTA updates

### Device Tracker
Import the device tracker blueprint into Home Assistant to display your car's location on the map

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2FShonP40%2FCar-Assistant%2Fblob%2Fmaster%2Fgps_device_tracker.yaml)

### Recover from GPS failures
Import the restart on GPS failure blueprint into Home Assistant to automatically restart your Car Assistant's modem & ESP32 if GPS fails and does not recover

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2FShonP40%2FCar-Assistant%2Fblob%2Fmaster%2Frestart_on_gps_fail.yaml)

## Credits
- [Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO/T-SIM7600X) - Basic T-SIM7600X implementation
- [oarcher](https://github.com/esphome/esphome/pull/6721#issuecomment-3362380809) - ESPHome SIM7600 implementation & Blueprint device tracker
- [n-serrette](https://github.com/n-serrette/esphome_sd_card) - ESPHome SDCard implementation
