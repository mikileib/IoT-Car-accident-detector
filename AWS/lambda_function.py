import json
import boto3
from datetime import datetime

timestream_client = boto3.client('timestream-write')
sns_client = boto3.client('sns')

# State variable to track if crash has been detected
crash_detected = False

def lambda_handler(event, context):
    global crash_detected

    try:
        # Extract relevant data from payload
        acceleration_x = event['acceleration_x']
        acceleration_y = event['acceleration_y']
        acceleration_z = event['acceleration_z']
        temperature = event['temperature']
        latitude = event['latitude']
        longitude = event['longitude']
        timestamp = event['timestamp']
        iso_timestamp_dt = datetime.strptime(timestamp, "%Y/%m/%d %H:%M:%S.%f")
        timestamp_ms = str(int(iso_timestamp_dt.timestamp() * 1000))
        
        # Store data in Timestream
        timestream_client.write_records(
            DatabaseName='Car_Database',
            TableName='Car_Table',
            Records=[
                {
                    'Dimensions': [{'Name': 'sensor_type', 'Value': 'accelerometer'}],
                    'MeasureName': 'acceleration_x',
                    'MeasureValue': str(acceleration_x),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp_ms
                },
                {
                    'Dimensions': [{'Name': 'sensor_type', 'Value': 'accelerometer'}],
                    'MeasureName': 'acceleration_y',
                    'MeasureValue': str(acceleration_y),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp_ms
                },
                {
                    'Dimensions': [{'Name': 'sensor_type', 'Value': 'accelerometer'}],
                    'MeasureName': 'acceleration_z',
                    'MeasureValue': str(acceleration_z),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp_ms
                },
                {
                    'Dimensions': [{'Name': 'sensor_type', 'Value': 'temperature'}],
                    'MeasureName': 'temperature',
                    'MeasureValue': str(temperature),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp_ms
                }
            ]
        )
        
        # Check for crash or fire
        if not crash_detected and (acceleration_y > 5 or acceleration_y < -5 or temperature > 90):
            if temperature > 90:
                send_fire_notification(latitude, longitude, timestamp)
            else:
                send_accident_notification(latitude, longitude, timestamp)
            crash_detected = True
        elif crash_detected and (-5 < acceleration_y < 5 and temperature <= 90):  # Reset crash detection
            crash_detected = False
            
    except KeyError as e:
        print(f"KeyError: {e}")
    except Exception as e:
        print(f"Exception: {e}")


def send_accident_notification(latitude, longitude, timestamp):
    map_link = f"https://maps.google.com/maps?q={latitude},{longitude}"
    message = f"Car accident detected at time: {timestamp}. Location: {map_link}"
    try:
        sns_client.publish(
            TopicArn='arn:aws:sns:eu-west-1:471112796953:Car_accident',
            Message=message
        )
        print("Accident notification sent successfully.")
    except Exception as e:
        print(f"Failed to send accident notification: {e}")


def send_fire_notification(latitude, longitude, timestamp):
    map_link = f"https://maps.google.com/maps?q={latitude},{longitude}"
    message = f"Car on fire detected at time: {timestamp}. Location: {map_link}"
    try:
        sns_client.publish(
            TopicArn='arn:aws:sns:eu-west-1:471112796953:Car_accident',
            Message=message
        )
        print("Fire notification sent successfully.")
    except Exception as e:
        print(f"Failed to send fire notification: {e}")
