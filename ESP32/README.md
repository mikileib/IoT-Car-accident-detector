# ESP32 Setup Guide

### Overview:
This project implements a data collection and transmission system for a car using an ESP32 microcontroller. The system collects sensor data including accelerometer readings, temperature, and GPS coordinates, and publishes this data to the AWS IoT platform via MQTT (Message Queuing Telemetry Transport) protocol over a secure WiFi connection.

## Hardware Requirements:
- ESP32 microcontroller board
- MPU6050 accelerometer and gyroscope sensor module
- GPS module (compatible with TinyGPS++)
- Wiring components
    
<p align="center">
  <img src="https://i.postimg.cc/FFSXLf1Y/Untitled-1.png" alt="Image Description">
</p>

## Software Dependencies:
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

## Setup Instructions:

1. Ensure all hardware components are properly connected to the ESP32 board as per the wiring configuration specified in the code.
2. Open the code in Arduino IDE.
3. Create a new file named `secrets.h` in the same directory as your main Arduino sketch file.
4. In the `secrets.h` file, define the following constants with your actual WiFi SSID, WiFi password, and AWS IoT endpoint information:
```cpp
#define WIFI_SSID "Your_Wifi_SSID"
#define WIFI_PASSWORD "Your_Wifi_Password"
#define AWS_IOT_ENDPOINT "Your_AWS_IoT_Endpoint"
```
5. Additionally, include the following AWS IoT certificates and keys in the `secrets.h` file. Replace the placeholder text with the contents of your certificates and keys:
```cpp
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
[Your Root CA Certificate]
-----END CERTIFICATE-----
)EOF";

static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
[Your Device Certificate]
-----END CERTIFICATE-----
)KEY";

static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
[Your Private Key]
-----END RSA PRIVATE KEY-----
)KEY";
```

6. Save the `secrets.h` file.
7. Upload the main code file to your ESP32 board.

## Operation:
1. Upon startup, the system initializes connections to WiFi and AWS IoT.
2. Sensor data including accelerometer readings, temperature, GPS coordinates, and timestamp are collected.
3. Data is packaged into a JSON format and published to the specified AWS IoT topic.
4. The system continuously checks for connectivity to WiFi and AWS IoT, attempting to reconnect if the connection is lost.
5. GPS data is continuously processed to ensure accurate location information.
6. LED status indication provides feedback on the system's operational status.

## Notes:
- Ensure the accuracy of sensor readings and GPS data by properly calibrating and configuring the hardware components.
- Monitor serial output for debugging and status information.
- Adjust parameters such as update intervals and MQTT topics according to your application requirements.

