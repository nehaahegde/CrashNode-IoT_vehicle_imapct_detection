#include <ESP8266WiFi.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <MPU6050.h>

// Sensor and GPS Configuration
MPU6050 mpu;
TinyGPSPlus gps;
SoftwareSerial ss(12, 13); // RX=D6, TX=D7 for GPS

// Crash Threshold (in Gs)
const float CRASH_THRESHOLD = 3.5; 
bool crashDetected = false;

// Network Configuration (For HTTP SMS Gateway)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* alertServer = "api.twilio.com"; // Example SMS Gateway

void setup() {
  Serial.begin(115200);
  ss.begin(9600);
  Wire.begin();
  
  Serial.println("Initializing CrashNode...");
  
  mpu.initialize();
  if(!mpu.testConnection()){
    Serial.println("MPU6050 connection failed!");
    while(1);
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nNetwork Connected.");
}

void loop() {
  // Feed GPS data
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Read Accelerometer Data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  
  // Calculate total acceleration vector magnitude (normalized to Gs)
  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;
  float totalG = sqrt((accX * accX) + (accY * accY) + (accZ * accZ));

  // Impact Detection
  if (totalG > CRASH_THRESHOLD && !crashDetected) {
    crashDetected = true;
    Serial.println("CRASH DETECTED! Initiating emergency protocol...");
    triggerEmergencyAlert();
  }

  // Reset flag manually or after a timeout for continuous monitoring
  delay(100);
}

void triggerEmergencyAlert() {
  String latitude = "UNAVAILABLE";
  String longitude = "UNAVAILABLE";
  
  if (gps.location.isValid()) {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
  }
  
  String alertMessage = "EMERGENCY: Vehicle Impact Detected. Location: " + latitude + ", " + longitude;
  Serial.println(alertMessage);
  
  // TODO: Add HTTP POST logic here to send SMS via your chosen API gateway
}
