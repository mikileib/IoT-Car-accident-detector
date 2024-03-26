## README: Car Data Collection and Transmission System

### Overview:
This project implements a data collection and transmission system for a car using an ESP32 microcontroller. The system collects sensor data including accelerometer readings, temperature, and GPS coordinates, and publishes this data to the AWS IoT platform via MQTT (Message Queuing Telemetry Transport) protocol over a secure WiFi connection.

### Hardware Requirements:
- ESP32 microcontroller board
- MPU6050 accelerometer and gyroscope sensor module
- GPS module (compatible with TinyGPS++)
- Red LED (for status indication)
- Wiring components

### Software Dependencies:
- Arduino IDE
- Libraries:
  - WiFiClientSecure.h
  - PubSubClient.h
  - ArduinoJson.h
  - WiFi.h
  - TinyGPS++.h
  - Adafruit_MPU6050.h
  - Adafruit_Sensor.h
  - Wire.h
  - SoftwareSerial.h

### Setup Instructions:
1. Ensure all hardware components are properly connected to the ESP32 board as per the wiring configuration specified in the code.
2. Open the code in Arduino IDE.
3. Create a new file named `secrets.h` in the same directory as your main Arduino sketch file.
4. In the `secrets.h` file, define the following constants with your actual WiFi SSID, WiFi password, and AWS IoT endpoint information:
   ```cpp
   #define WIFI_SSID "Your_Wifi_SSID"
   #define WIFI_PASSWORD "Your_Wifi_Password"
   #define AWS_IOT_ENDPOINT "Your_AWS_IoT_Endpoint"
