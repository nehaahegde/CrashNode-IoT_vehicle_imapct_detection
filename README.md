# CrashNode: IoT Vehicle Impact Detection System

CrashNode is an embedded IoT solution designed to automatically detect vehicle collisions and immediately dispatch emergency alerts containing precise geolocation data. 

## Hardware Architecture
*   **Microcontroller:** ESP8266 NodeMCU
*   **Accelerometer:** MPU6050 (Impact threshold detection)
*   **GPS Module:** NEO-6M (Real-time location tracking)

## Software & Libraries
*   C++ / Arduino IDE
*   `TinyGPS++.h` for NMEA sentence parsing
*   `MPU6050.h` for I2C accelerometer data retrieval

## System Workflow
1.  **Monitor:** The ESP8266 continuously reads 3-axis acceleration data from the MPU6050.
2.  **Evaluate:** Calculates the total acceleration vector magnitude. If the magnitude exceeds the configured `CRASH_THRESHOLD` (e.g., 3.5 Gs), an impact is registered.
3.  **Locate:** Retrieves the latest valid latitude and longitude from the NEO-6M GPS module.
4.  **Alert:** Triggers the emergency protocol, transmitting the alert and coordinates over Wi-Fi to a connected SMS gateway (e.g., Twilio).

## Setup & Deployment
1.  Connect the MPU6050 (I2C) and NEO-6M (Serial TX/RX to D6/D7) to the ESP8266.
2.  Update the `ssid` and `password` variables in `crashnode_firmware.ino` with your network credentials.
3.  Flash the firmware using the Arduino IDE (ensure the ESP8266 board package is installed).
4.  Monitor the Serial Output (115200 baud) for initialization and status logs.

---
**Author:** Neha Umapati Hegde
