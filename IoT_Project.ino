#include "secrets.h" // Include file for WiFi and AWS IoT credentials
#include <WiFiClientSecure.h> // Library for secure WiFi client
#include <PubSubClient.h> // Library for MQTT client
#include <ArduinoJson.h> // Library for JSON serialization and deserialization
#include <WiFi.h> // Library for WiFi connectivity
#include <TinyGPS++.h> // Library for GPS parsing
#include <Adafruit_MPU6050.h> // Library for MPU6050 sensor
#include <Adafruit_Sensor.h> // Library for sensor functions
#include <Wire.h> // Library for I2C communication
#include <SoftwareSerial.h> // Library for software serial communication

#define AWS_IOT_PUBLISH_TOPIC "Car/data" // MQTT topic for publishing data
#define AWS_IOT_SUBSCRIBE_TOPIC "Car/sub" // MQTT topic for subscribing to messages

static const int RXPin = 17, TXPin = 16; // Pins for software serial communication with GPS module
static const uint32_t GPSBaud = 9600; // Baud rate for GPS module communication

const float FAKE_LATITUDE = 31.262056; // When GPS not working generate location
const float FAKE_LONGITUDE = 34.803872;
int utcOffsetHours = -1; // Israel standard time UTC offset (+2)

const int RED_LED_PIN = 2; // Pin for the red LED

WiFiClientSecure net; // Secure WiFi client
PubSubClient client(net); // MQTT client

TinyGPSPlus gps; // GPS object
Adafruit_MPU6050 mpu; // MPU object
SoftwareSerial ss(RXPin, TXPin); // Software serial connection to the GPS device

// Function to connect to AWS IoT
void connectAWS() {
  digitalWrite(RED_LED_PIN, HIGH); // Turn on red LED
  
  WiFi.mode(WIFI_STA); // Set WiFi mode to station mode
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to WiFi network
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED) { // Wait until WiFi is connected
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Set up message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IoT");
 
  while (!client.connect(THINGNAME)) { // Connect to AWS IoT
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected()) { // Check if connection to AWS IoT failed
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");

  digitalWrite(RED_LED_PIN, LOW); // Turn off red LED
}

// Function to publish message to AWS IoT
void publishMessage(float accelX, float accelY, float accelZ, float temperature, float latitude, float longitude, const char* timestamp) {
  DynamicJsonDocument doc(1024); // Create a JSON document
  
  // Add sensor data to JSON document
  doc["acceleration_x"] = accelX;
  doc["acceleration_y"] = accelY;
  doc["acceleration_z"] = accelZ;
  doc["temperature"] = temperature;
  doc["latitude"] = latitude;
  doc["longitude"] = longitude;
  doc["timestamp"] = timestamp;

  char jsonBuffer[1024];
  serializeJson(doc, jsonBuffer); // Serialize JSON to char array
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer); // Publish message to AWS IoT topic
}

// Function to handle incoming MQTT messages
void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("incoming: ");
  Serial.println(topic);
 
  DynamicJsonDocument doc(512);
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

// Function to display GPS information
void displayGPSInfo() {
  
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("Fake Data: "));
    Serial.print(FAKE_LATITUDE, 6);
    Serial.print(F(","));
    Serial.print(FAKE_LONGITUDE, 6);
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print((gps.time.hour() + utcOffsetHours) % 24); // Apply UTC offset to hours
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

// Function to display MPU information
void displayMPUInfo(const sensors_event_t& a, const sensors_event_t& g, const sensors_event_t& temp) {
  // Print out the accelerometer values
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  // Print out the gyroscope values
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  // Print out the temperature
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
}

// Custom delay function to ensure continuous GPS data processing
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void checkAWSConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    digitalWrite(RED_LED_PIN, HIGH); // Turn on red LED to indicate WiFi reconnection attempt
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi reconnected.");
    digitalWrite(RED_LED_PIN, LOW); // Turn off red LED
  }
  
  if (!client.connected()) {
    Serial.println("Disconnected from AWS IoT. Reconnecting...");
    digitalWrite(RED_LED_PIN, HIGH); // Turn on red LED to indicate AWS IoT reconnection attempt
    
    while (!client.connected()) {
      if (client.connect(THINGNAME)) {
        Serial.println("Reconnected to AWS IoT.");
        digitalWrite(RED_LED_PIN, LOW); // Turn off red LED
        client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
      } else {
        Serial.print(".");
        delay(1000);
      }
    }
  }
}


void setup() {
  pinMode(RED_LED_PIN, OUTPUT); // Initialize red LED pin as output
  digitalWrite(RED_LED_PIN, HIGH); // Turn on red LED
  
  Serial.begin(115200); // Start serial communication
  ss.begin(GPSBaud); // Start software serial communication with GPS module
  Wire.begin(); // Begin I2C communication
  mpu.begin(); // Begin MPU6050 sensor communication
  
  while (!Serial)
    delay(10); // Pause until serial console opens
  
  Serial.println("Lets START!");
  connectAWS(); // Connect to AWS IoT
}

void loop() {
  displayGPSInfo(); // Display GPS information
  // Get new sensor events with the accelerometer readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  displayMPUInfo(a, g, temp); // Display MPU information
  
  // Get timestamp from GPS
  char timestamp[32];
  snprintf(timestamp, sizeof(timestamp), "%d/%d/%d %02d:%02d:%02d.%02d", 
         gps.date.year(), 
         gps.date.month(), 
         gps.date.day(), 
         (gps.time.hour() + utcOffsetHours) % 24, 
         gps.time.minute(), 
         gps.time.second(), 
         gps.time.centisecond());

    // Check if GPS data is valid
  float currentLatitude, currentLongitude;
  if (gps.location.isValid()) {
      currentLatitude = gps.location.lat();
      currentLongitude = gps.location.lng();
  } else {
      currentLatitude = FAKE_LATITUDE;
      currentLongitude = FAKE_LONGITUDE;
  }

  // Publish data to AWS IoT
  publishMessage(a.acceleration.x, a.acceleration.y, a.acceleration.z, temp.temperature, currentLatitude, currentLongitude, timestamp);

  client.loop();
  
  checkAWSConnection(); // Check MQTT connection and reconnect if necessary

  smartDelay(1000); // Delay to allow GPS data processing

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
