# IoT Health Assistant

The IoT Health Assistant is an ESP32-based project that monitors vital health parameters, including heart rate, blood oxygen level (SpO2), temperature, and humidity, and sends the data to the Blynk IoT platform for real-time visualization.

## Features
- **Heart Rate & SpO2 Monitoring**: Uses the MAX30105 sensor to measure heart rate (bpm) and blood oxygen saturation (%).
- **Temperature Monitoring**: Measures temperature using DS18B20 (in °F) and DHT11 (in °C) sensors.
- **Humidity Monitoring**: Measures humidity (%) using the DHT11 sensor.
- **Real-Time Data**: Sends sensor data to the Blynk app every 2 seconds.
- **WiFi Connectivity**: Utilizes the ESP32’s WiFi capabilities for cloud integration.

## Hardware Requirements
- ESP32 Development Board (e.g., ESP32-WROOM-32)
- MAX30105 Pulse Oximeter Sensor
- DS18B20 Temperature Sensor (with 4.7kΩ pull-up resistor)
- DHT11 Temperature and Humidity Sensor
- Jumper wires, breadboard, and power supply (3.3V or 5V)

## Software Requirements
- Arduino IDE (version 2.x recommended)
- Blynk App (iOS or Android) for data visualization
- Arduino libraries:
  - `Wire.h` (built-in)
  - `MAX30105.h` by SparkFun
  - `heartRate.h` by SparkFun
  - `spo2_algorithm.h` by SparkFun
  - `OneWire.h` by Jim Studt
  - `DallasTemperature.h` by Miles Burton
  - `DHT.h` by Adafruit
  - `WiFi.h` (included with ESP32 board package)
  - `BlynkSimpleEsp32.h` by Volodymyr Shymanskyy

## Wiring Diagram
| Sensor       | ESP32 Pin | Notes                              |
|--------------|-----------|------------------------------------|
| MAX30105 SDA | GPIO 21   | I2C data line                     |
| MAX30105 SCL | GPIO 22   | I2C clock line                    |
| DS18B20      | GPIO 19   | OneWire bus, 4.7kΩ pull-up needed |
| DHT11        | GPIO 18   | Data pin                          |
| VCC          | 3.3V/5V   | Check sensor voltage requirements |
| GND          | GND       | Common ground                     |

## Setup Instructions
1. **Install Arduino IDE**:
   - Download from [arduino.cc](https://www.arduino.cc/en/software).
   - Add ESP32 board support: Follow [Espressif’s guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

2. **Install Libraries**:
   - Open Arduino IDE → Sketch → Include Library → Manage Libraries.
   - Search and install:
     - `SparkFun MAX3010x Sensor Library`
     - `OneWire`
     - `DallasTemperature`
     - `DHT sensor library`
     - `Blynk`
   - Alternatively, download libraries from their respective GitHub repositories.

3. **Configure Blynk**:
   - Sign up at [Blynk](https://blynk.io/) and create a new device.
   - Use the provided `BLYNK_TEMPLATE_ID` and `BLYNK_AUTH_TOKEN` in `secrets.h`.
   - Update `WIFI_SSID` and `WIFI_PASS` in `secrets.h` with your WiFi credentials.

4. **Upload Code**:
   - Open `IoT_Health_Assistant.ino` in the Arduino IDE.
   - Place `secrets.h` in the same folder as the `.ino` file.
   - Select your ESP32 board and port in the IDE.
   - Upload the code.

5. **Set Up Blynk App**:
   - In the Blynk app, create a dashboard with widgets for:
     - `V1`: DS18B20 temperature (°F)
     - `V2`: DHT11 temperature (°C)
     - `V3`: DHT11 humidity (%)
     - `V4`: Heart rate (bpm)
     - `V5`: SpO2 (%)
   - Ensure the device is online in the app.

## Usage
- Power on the ESP32 and place your finger on the MAX30105 sensor to measure heart rate and SpO2.
- Sensor data is sent to Blynk every 2 seconds.
- Monitor real-time data in the Blynk app or Serial Monitor (115200 baud).

## Troubleshooting
- **MAX30105 not detected**: Check I2C wiring (SDA/SCL) and power supply. Ensure the sensor is compatible with 3.3V/5V.
- **No Blynk connection**: Verify WiFi credentials in `secrets.h` and ensure the Blynk server is accessible.
- **Invalid sensor readings**: Check sensor wiring and ensure libraries are correctly installed.
- **Serial Monitor issues**: Set baud rate to 115200 in the Arduino IDE.

## Contributing
Contributions are welcome! Please:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/your-feature`).
3. Commit changes (`git commit -m "Add your feature"`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
- [SparkFun](https://github.com/sparkfun) for MAX30105 libraries.
- [Blynk](https://blynk.io/) for IoT platform support.
- [Adafruit](https://github.com/adafruit) for DHT library.

- 
## 🧠 Author  
**Md Shaifulla Zibon**  
Electrical and Electronics Engineering  
European University of Bangladesh  
GitHub: [@zibontaroka](https://github.com/zibontaroka)
