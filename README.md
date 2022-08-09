# Car Assistant
Keep track of your car remotely through Home Assistant

# Features
- Detailed user config file (rename `config.h.example` to `config.h`)
- Cellular connectivity
- Automatically update the ESP's clock
- MQTT connection
- Report battery status (percentage & voltage)
- Report network details

# Software Requirements
- IDE of choise with PlatformIO (VSCode is recommended)
- USB <-> Serial driver ([`macOS`](https://github.com/Xinyuan-LilyGO/CH9102_Mac_Driver) | [`Linux`](https://github.com/gorgiaxx/CH34x-Driver-Linux) | [`Windows`](https://github.com/Xinyuan-LilyGO/CH9102_Driver))
- MQTT Broker

# Hardware Requirements
- LilyGO® TTGO T-SIM7600E-L1C ESP32 (Other ESP32 TTGO models could also work)
- A Nano SIM Card with an active internet plan
- Cellular antenna
- GPS antenna
- 18650B battery

# Credits
[Xinyuan-LilyGO](https://github.com/Xinyuan-LilyGO/T-SIM7600X) - Basic T-SIM7600X implementation