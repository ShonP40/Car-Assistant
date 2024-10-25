const char* index_html = R"literal(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Car Assistant</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <style>
        html {
            font-family: Arial, Helvetica, sans-serif;
            display: inline-block;
            text-align: center;
        }

        h1 {
            font-size: 1.8rem;
            color: white;
        }

        p {
            font-size: 1.4rem;
        }

        .topnav {
            overflow: hidden;
            background-color: #193693;
        }

        body {
            margin: 0;
        }

        .content {
            padding: 5%;
        }

        .card-grid {
            max-width: 800px;
            margin: 0 auto;
            display: grid;
            grid-gap: 2rem;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
        }

        .card {
            background-color: white;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

        .card-title {
            font-size: 1.2rem;
            font-weight: bold;
            color: #034078
        }

        input[type=submit] {
            border: none;
            color: #FEFCFB;
            background-color: #034078;
            padding: 15px 15px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            width: 100px;
            margin-right: 10px;
            border-radius: 4px;
            transition-duration: 0.4s;
        }

        input[type=submit]:hover {
            background-color: #1282A2;
        }

        input[type=text], input[type=number], select {
            width: 50%;
            padding: 12px 20px;
            margin: 18px;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        label {
            font-size: 1.2rem;
        }

        .value {
            font-size: 1.2rem;
            color: #1282A2;
        }

        .state {
            font-size: 1.2rem;
            color: #1282A2;
        }

        button {
            border: none;
            color: #FEFCFB;
            padding: 15px 32px;
            text-align: center;
            font-size: 16px;
            border-radius: 4px;
            transition-duration: 0.4s;
        }

        .button-on {
            background-color: #034078;
        }

        .button-on:hover {
            background-color: #1282A2;
        }

        .button-off {
            background-color: #858585;
        }

        .button-off:hover {
            background-color: #252524;
        }

        footer {
            background-color: #034078;
            padding: 10px;
            color: white;
            font-size: 16px;
        }

        a:link {
            color: rgb(0, 255, 251);
            background-color: transparent;
            text-decoration: none;
        }

        a:visited {
            color: rgb(0, 255, 251);
            background-color: transparent;
            text-decoration: none;
        }

        a:hover {
            color: rgb(173, 157, 41);
            background-color: transparent;
            text-decoration: underline;
        }

        a:active {
            color: yellow;
            background-color: transparent;
            text-decoration: underline;
        }
    </style>
    <body>
        <div class="topnav">
            <h1>Car Assistant &bull; <a href="update">Update</a> | <a href="/restart">Restart</a></h1>
        </div>
        <div class="content">
            <div class="card-grid">
                <div class="card">
                    <div class="card-title">
                        <h2>Configuration</h2>
                    </div>
                    <form id="config" action="/" method="POST">
                        <p> Cellular </br>
                            <label for="apn">APN</label>
                            <input type="text" id="apn" name="apn" value="yourapn.com">
                            <br>
                            <label for="apnusername">Username</label>
                            <input type="text" id="apnusername" name="apnusername" value="user">
                            <br>
                            <label for="apnpassword">Password</label>
                            <input type="text" id="apnpassword" name="apnpassword" value="pass">
                            <br>
                            <label for="simpin">SIM PIN</label>
                            <input type="text" id="simpin" name="simpin">
                        </p>
                        <p> Clock </br>
                            <label for="timezone">Timezone</label>
                            <input type="text" id="timezone" name="timezone" value="7200">
                            <br>
                            <label for="dst">Daylight Saving Time</label>
                            <input type="text" id="dst" name="dst" value="true">
                        </p>
                        <p> WiFi AP </br>
                            <label for="apssid">SSID</label>
                            <input type="text" id="apssid" name="apssid" value="Car Assistant">
                            <br>
                            <label for="appassword">Password</label>
                            <input type="text" id="appassword" name="appassword" value="12345678">
                            <br>
                            <label for="appowersaving">Disable when on battery or stationary?</label>
                            <input type="text" id="appowersaving" name="appowersaving" value="true">
                        </p>
                        <p> MQTT Broker </br>
                            <label for="mqttaddress">Address</label>
                            <input type="text" id="mqttaddress" name="mqttaddress" value="yourbroker.com">
                            <br>
                            <label for="mqttport">Port</label>
                            <input type="text" id="mqttport" name="mqttport" value="1883">
                            <br>
                            <label for="mqttclientname">Client name</label>
                            <input type="text" id="mqttclientname" name="mqttclientname" value="car-assistant">
                            <br>
                            <label for="mqttusername">Username</label>
                            <input type="text" id="mqttusername" name="mqttusername" value="user">
                            <br>
                            <label for="mqttpassword">Password</label>
                            <input type="text" id="mqttpassword" name="mqttpassword" value="pass">
                        </p>
                        <p> Location </br>
                            <label for="locationgnssmode">GNSS Mode</label>
                            <input type="text" id="locationgnssmode" name="locationgnssmode" value="0">
                            <br>
                            <label for="locationdpo">DPO</label>
                            <input type="text" id="locationdpo" name="locationdpo" value="1">
                        </p>
                        <p> Sensors </br>
                            <label for="sensorsenable">Enable?</label>
                            <input type="text" id="sensorsenable" name="sensorsenable" value="true">
                        </p>
                        <p> BME280 </br>
                            <label for="sensorsenablebme280">Enable?</label>
                            <input type="text" id="sensorsenablebme280" name="sensorsenablebme280" value="true">
                            <br>
                            <label for="sensorbme280i2caddress">I2C Address</label>
                            <input type="text" id="sensorbme280i2caddress" name="sensorbme280i2caddress" value="0x76">
                            <br>
                            <label for="mqttsensorsbme280temperature">MQTT Temp</label>
                            <input type="text" id="mqttsensorsbme280temperature" name="mqttsensorsbme280temperature" value="temperature">
                            <br>
                            <label for="mqttsensorsbme280pressure">MQTT Pressure</label>
                            <input type="text" id="mqttsensorsbme280pressure" name="mqttsensorsbme280pressure" value="pressure">
                            <br>
                            <label for="mqttsensorsbme280humidity">MQTT Humidity</label>
                            <input type="text" id="mqttsensorsbme280humidity" name="mqttsensorsbme280humidity" value="humidity">
                        </p>
                        <p> TSL2561 </br>
                            <label for="sensorsenabletsl2561">Enable?</label>
                            <input type="text" id="sensorsenabletsl2561" name="sensorsenabletsl2561" value="true">
                            <br>
                            <label for="sensortsl2561i2caddress">I2C Address</label>
                            <input type="text" id="sensortsl2561i2caddress" name="sensortsl2561i2caddress" value="0x39">
                            <br>
                            <label for="sensortsl2561gain">Gain</label>
                            <input type="text" id="sensortsl2561gain" name="sensortsl2561gain" value="auto">
                            <br>
                            <label for="mqttsensorstsl2561lux">MQTT</label>
                            <input type="text" id="mqttsensorstsl2561lux" name="mqttsensorstsl2561lux" value="lux">
                        </p>
                        <p> PIR </br>
                            <label for="sensorsenablepir">Enable?</label>
                            <input type="text" id="sensorsenablepir" name="sensorsenablepir" value="true">
                            <br>
                            <label for="sensorpirpin">GPIO</label>
                            <input type="text" id="sensorpirpin" name="sensorpirpin" value="19">
                            <br>
                            <label for="mqttsensorspir">MQTT</label>
                            <input type="text" id="mqttsensorspir" name="mqttsensorspir" value="pir">
                        </p>
                        <p> CPU Temperature </br>
                            <label for="sensorsenablecputemp">Enable?</label>
                            <input type="text" id="sensorsenablecputemp" name="sensorsenablecputemp" value="true">
                            <br>
                            <label for="mqttsensorscputemp">MQTT</label>
                            <input type="text" id="mqttsensorscputemp" name="mqttsensorscputemp" value="cpu-temperature">
                        </p>
                        <p> CPU Frequency </br>
                            <label for="sensorsenablecpufreq">Enable?</label>
                            <input type="text" id="sensorsenablecpufreq" name="sensorsenablecpufreq" value="true">
                            <br>
                            <label for="mqttsensorscpufreq">MQTT</label>
                            <input type="text" id="mqttsensorscpufreq" name="mqttsensorscpufreq" value="cpu-frequency">
                        </p>
                        <p> Free RAM </br>
                            <label for="sensorsenablefreeram">Enable?</label>
                            <input type="text" id="sensorsenablefreeram" name="sensorsenablefreeram" value="true">
                            <br>
                            <label for="mqttsensorsfreeram">MQTT</label>
                            <input type="text" id="mqttsensorsfreeram" name="mqttsensorsfreeram" value="free-ram">
                        </p>
                        <p> Hall </br>
                            <label for="sensorsenablehall">Enable?</label>
                            <input type="text" id="sensorsenablehall" name="sensorsenablehall" value="true">
                            <br>
                            <label for="mqttsensorshall">MQTT</label>
                            <input type="text" id="mqttsensorshall" name="mqttsensorshall" value="hall">
                        </p>
                        <p> Modem MQTT topics </br>
                            <label for="mqttmodeminfo">Info</label>
                            <input type="text" id="mqttmodeminfo" name="mqttmodeminfo" value="modem-info">
                            <br>
                            <label for="mqttmodemccid">CCID</label>
                            <input type="text" id="mqttmodemccid" name="mqttmodemccid" value="ccid">
                            <br>
                            <label for="mqttmodemimsi">IMSI</label>
                            <input type="text" id="mqttmodemimsi" name="mqttmodemimsi" value="imsi">
                            <br>
                            <label for="mqttmodemoperator">Operator</label>
                            <input type="text" id="mqttmodemoperator" name="mqttmodemoperator" value="operator">
                            <br>
                            <label for="mqttmodemsignalquality">Signal Quality</label>
                            <input type="text" id="mqttmodemsignalquality" name="mqttmodemsignalquality" value="signal-quality">
                            <br>
                            <label for="mqttmodempublicip">Public IP</label>
                            <input type="text" id="mqttmodempublicip" name="mqttmodempublicip" value="public-ip">
                        </p>
                        <p> Battery MQTT topics </br>
                            <label for="mqttbatterypercentage">Percentage</label>
                            <input type="text" id="mqttbatterypercentage" name="mqttbatterypercentage" value="battery-percentage">
                            <br>
                            <label for="mqttbatteryvoltage">Voltage</label>
                            <input type="text" id="mqttbatteryvoltage" name="mqttbatteryvoltage" value="battery-voltage">
                            <br>
                            <label for="mqttbatterystatus">Status</label>
                            <input type="text" id="mqttbatterystatus" name="mqttbatterystatus" value="battery-status">
                        </p>
                        <p> Location MQTT topics </br>
                            <label for="mqttlocationtype">Type</label>
                            <input type="text" id="mqttlocationtype" name="mqttlocationtype" value="location-type">
                            <br>
                            <label for="mqttlocationlatitude">Latitude</label>
                            <input type="text" id="mqttlocationlatitude" name="mqttlocationlatitude" value="latitude">
                            <br>
                            <label for="mqttlocationlongitude">Longitude</label>
                            <input type="text" id="mqttlocationlongitude" name="mqttlocationlongitude" value="longitude">
                            <br>
                            <label for="mqttlocationspeed">Speed</label>
                            <input type="text" id="mqttlocationspeed" name="mqttlocationspeed" value="speed">
                            <br>
                            <label for="mqttlocationaltitude">Altitude</label>
                            <input type="text" id="mqttlocationaltitude" name="mqttlocationaltitude" value="altitude">
                            <br>
                            <label for="mqttlocationaccuracy">Accuracy</label>
                            <input type="text" id="mqttlocationaccuracy" name="mqttlocationaccuracy" value="location-accuracy">
                        </p>
                        <p> FTP </br>
                            <label for="ftpenable">Enable?</label>
                            <input type="text" id="ftpenable" name="ftpenable" value="false">
                            <br>
                            <label for="ftpuser">Username</label>
                            <input type="text" id="ftpuser" name="ftpuser" value="carassistant">
                            <br>
                            <label for="ftppass">Password</label>
                            <input type="text" id="ftppass" name="ftppass" value="12345678">
                        </p>
                        <p> Misc MQTT topics </br>
                            <label for="mqttuptime">Uptime</label>
                            <input type="text" id="mqttuptime" name="mqttuptime" value="uptime">
                            <br>
                            <label for="mqttversion">Version</label>
                            <input type="text" id="mqttversion" name="mqttversion" value="version">
                        </p>
                        <p> Power Saving </br>
                            <label for="dynamicfrequency">Dynamic Frequency?</label>
                            <input type="text" id="dynamicfrequency" name="dynamicfrequency" value="false">
                            <br>
                            <label for="lowpowermodeonbattery">Low Power mode on battery?</label>
                            <input type="text" id="lowpowermodeonbattery" name="lowpowermodeonbattery" value="false">
                            <br>
                            <input type="submit" value="Submit">
                        </p>
                    </form>
                </div>
            </div>
        </div>
        <footer>
            <div class="container">
                Car Assistant by ShonP40 | <a href="https://shon.codes">Website</a> | <a href="https://github.com/ShonP40/Car-Assistant">Repository</a> | Version: <span id="version"></span>
            </div>
        </footer>
    </body>
    <script>
        var list = {};
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "config.json", false);
        xhr.send();
        if (xhr.status == 200) {
            list = JSON.parse(xhr.responseText);
        }
        for (var key in list) {
            if (list.hasOwnProperty(key)) {
                var value = list[key];
                var element = document.getElementById("config").elements[key];
                if (element) {
                    element.value = value;
                }
            }
        }
    </script>
    <script>
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "version.json", false);
        xhr.send();
        if (xhr.status == 200) {
            var version = JSON.parse(xhr.responseText);
            document.getElementById("version").innerHTML = version.version;
        }
    </script>
    </html>
)literal";