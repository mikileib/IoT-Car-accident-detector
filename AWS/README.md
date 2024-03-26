# AWS Setup Guide

### Overview:

This guide provides detailed instructions for setting up the AWS (Amazon Web Services) components required for the IoT Car Accident Detection System project.

## AWS Architecture

Below is a simplified diagram illustrating the architecture of the IoT Car Accident Detection System on AWS:

<div align="center">
  <img src="https://i.postimg.cc/LsT41JXd/Untitled-2.png" alt="AWS Architecture">
</div>

## Prerequisites

Before proceeding with the AWS setup, ensure you have the following prerequisites:

- **AWS Account**: You need an AWS account to access AWS services.
- **Basic Familiarity**: Basic familiarity with AWS services and console navigation is recommended.

## Setup Steps

Follow these steps to set up the necessary AWS components:

### 1. AWS IoT Core Setup

- **Create IoT Thing**: Set up an AWS IoT Core instance and create an IoT Thing to represent your ESP32 microcontroller.
- **Define Thing Type and Attributes**: Define the type and attributes for your IoT Thing, specifying the sensor data it will send.
- **Generate Certificates and Keys**: Generate certificates and keys for your IoT Thing to enable secure communication.
- **Set Up Policy**: Create an AWS IoT policy to grant necessary permissions for your IoT Thing to connect and publish messages.
- **Create and Attach Policy**: Create the policy and attach it to your IoT Thing to authorize its actions.

### 2. AWS Lambda Function Setup

- **Create Lambda Function**: Set up a new AWS Lambda function to process incoming messages from the IoT Thing.
- **Select Runtime and Trigger**: Choose Python as the runtime and configure the trigger to be AWS IoT Core.
- **Configure Environment Variables**: Define environment variables for AWS service configurations and resource ARNs.
- **Paste Code**: Copy and paste the provided Python code `lambda_function.py` into the Lambda function editor.
- **Ensure Permissions**: Ensure that the Lambda function has permissions to interact with other AWS services such as Timestream and SNS.

### 3. Amazon Timestream Configuration

- **Create Database and Table**: Create a new database and table in Amazon Timestream to store sensor data.
- **Define Table Schema**: Define the schema for the Timestream table to match the data structure expected from the Lambda function.
- **Set Up Data Retention Policy**: Configure the data retention policy for the Timestream database to manage data storage.

### 4. Amazon SNS Setup

- **Create SNS Topic**: Set up an Amazon SNS topic to handle notifications for car accidents and fires.
- **Configure Subscriptions**: Configure subscriptions to the SNS topic to send notifications to specified endpoints such as email addresses or mobile devices.
- **Set Up Access Policies**: Define access policies to control who can publish messages to the SNS topic.

### 5. Testing and Validation

- **Simulate Data**: Test the integration by simulating data from the ESP32 microcontroller and verifying that messages are successfully received by AWS IoT Core.
- **Monitor Lambda Execution**: Monitor the execution of the Lambda function and verify that data is being stored in Amazon Timestream.
- **Verify Notifications**: Ensure that notifications are being sent out correctly via Amazon SNS when car accidents or fires are detected.

### 6. Grafana Integration

- **Add Timestream Data Source**: Configure Grafana to connect to Amazon Timestream as a data source.
- **Create Dashboards**: Use Grafana's dashboard editor to create custom dashboards displaying relevant metrics and trends from the Timestream database.
- **Add Panels**: Add panels to the dashboard to visualize data such as acceleration, temperature.
- **Share Dashboards**: Share dashboards with team members or stakeholders to provide visibility into the real-time monitoring and analysis of the IoT Car Accident Detection System.

## Conclusion

By following these setup instructions, you can configure the AWS components necessary for the IoT Car Accident Detection System project. Once set up, the system will be capable of real-time monitoring and rapid response in the event of a car accident.
