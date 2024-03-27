<h1 align="center" id="title">IoT Car accident detector</h1>

  <p align="center">
    Michael Leib and Yuval Yacov Said IoT Final Project

<p align="center"><img src="https://i.postimg.cc/t4zBdHxx/accident.png" alt="project-image"></p>


## About The Project

Our IoT Car Accident Detection System is a cutting-edge project designed to enhance road safety and emergency response mechanisms. Leveraging the power of ESP32 microcontrollers and AWS (Amazon Web Services) cloud services, this system aims to provide real-time monitoring and rapid response capabilities in the event of a car accident.

### Key Features:

**ESP32 Microcontroller Integration:** The project utilizes ESP32 microcontrollers for data collection, processing, and communication. With its powerful capabilities and built-in Wi-Fi and Bluetooth connectivity, ESP32 serves as the primary hardware platform for sensor integration and data transmission.

**Sensor Integration:** Various sensors are incorporated into the system to gather crucial data related to vehicle motion, orientation, and location. This includes the MPU6050 accelerometer and gyroscope sensor for motion detection, as well as the GY-GPS6MV2 GPS module for accurate positioning.

**Real-time Data Streaming to AWS IoT Core:** The collected sensor data, including data from the MPU6050 and GY-GPS6MV2 module, is securely transmitted to the AWS IoT Core, a managed cloud service that enables seamless communication between IoT devices and the cloud. This allows for real-time streaming of sensor data to the cloud infrastructure for further analysis and processing.

**Event Detection and Alert Generation:** AWS cloud environment is utilized to analyze incoming sensor data in real-time. In the event of a detected car accident, the system triggers instant alerts and notifications to designated emergency contacts, including emergency services, family members, and relevant authorities.

**Data Logging and Historical Analysis:** All sensor data, including pre- and post-accident information, is logged and stored in AWS databases for historical analysis and forensic purposes. This data can be accessed and visualized through custom dashboards and analytics tools, providing valuable insights for accident reconstruction and safety improvements.


# Getting Started

Welcome to the IoT Car Accident Detection System project! This guide will help you get started with setting up and configuring the system.

## Prerequisites

### Hardware Components:

* ESP32 microcontroller board
* MPU6050 accelerometer and gyroscope sensor
* GY-GPS6MV2 GPS module
* Power source (battery or USB)
* Necessary connecting wires and breadboard (if required)

### Development Environment:

* Arduino IDE or PlatformIO with ESP-IDF for programming the ESP32 microcontroller.
* Required libraries and dependencies for interfacing with the MPU6050 sensor, GY-GPS6MV2 module, and AWS services.

### AWS Account:

* An AWS account to set up AWS IoT Core, Lambda functions, Timestream databases and Simple Notification Service.
* Basic familiarity with AWS services and console navigation.


## Installation


### AWS IoT Core Setup:

* Set up an AWS IoT Core instance to receive MQTT messages from the ESP32 microcontroller.
* Create a rule in AWS IoT Core to trigger a Lambda function upon receiving messages from the ESP32.

### AWS Lambda Function Setup:

* Create a new Lambda function in the AWS Lambda console.
* Copy and paste the provided Python code into the Lambda function editor.
* Update the code with the appropriate AWS service client configurations and resource ARNs.
* Ensure the Lambda function has permissions to write data to Amazon Timestream and publish messages to Amazon SNS.

### Amazon Timestream Configuration:

* Create a new database and table in Amazon Timestream to store the sensor data received from the ESP32 microcontroller.
* Define the schema for the Timestream table to match the data structure expected from the Lambda function.

### Amazon SNS Setup:

* Create an Amazon SNS topic to handle notifications for car accidents and fires detected by the Lambda function.
* Configure subscriptions to the SNS topic to send notifications to specified endpoints, such as email addresses or mobile devices.

### Testing and Validation:

* Test the integration by simulating data from the ESP32 microcontroller and verifying that messages are successfully received by AWS IoT Core.
* Monitor the execution of the Lambda function and verify that data is being stored in Amazon Timestream.
* Verify that notifications are being sent out correctly via Amazon SNS when car accidents or fires are detected.

### Grafana Integration:

* Set up a connection between Grafana and Amazon Timestream to visualize and analyze the stored sensor data.
* Configure Grafana dashboards to display relevant metrics and trends from the Timestream database.
