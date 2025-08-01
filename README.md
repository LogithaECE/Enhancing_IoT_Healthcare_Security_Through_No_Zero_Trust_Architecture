# IoT-Based Pacemaker Monitoring System with NZTA

A secure, real-time health monitoring system for patients with pacemakers using a heartbeat sensor and NodeMCU. This project integrates **No Zero Trust Architecture (NZTA)** principles to ensure secure communication, authentication, and data integrity. Data is transmitted to cloud platforms such as **ThingSpeak** or **Adafruit IO** for continuous monitoring and anomaly alerts.

## Overview

Implantable Medical Devices (IMDs) are vulnerable to cybersecurity threats. Traditional systems often lack proper data protection, risking both patient safety and data integrity.  
This project introduces an **NZTA-inspired architecture**, ensuring that:

- Every device and data exchange is verified  
- No default trust is assumed  
- Secure cloud APIs are used for real-time monitoring  
- Alerts are generated for cardiac anomalies and interference

## Key Features

• Real-time cardiac data acquisition using heartbeat sensor + NodeMCU  
• Secure transmission to cloud platforms (ThingSpeak / Adafruit IO)  
• NZTA-based security: trust-by-verification, no implicit trust  
• Automatic alerts for irregular heartbeats or EMI disturbances  
• Simple, scalable, and hardware-friendly implementation

## Hardware Components

• NodeMCU ESP8266 (microcontroller with Wi-Fi)  
• Heartbeat sensor module (analog pulse detection)  
• Breadboard and jumper wires  
• Wi-Fi connection  
• Optional: LED/Buzzer for local alerts


##  Software & Tools Used

• Arduino IDE (for coding and uploading)  
• ThingSpeak or Adafruit IO (cloud dashboard)  
• Embedded C / Arduino Sketch  
• Optional: MQTT protocol with TLS (future integration)

## Setup Instructions

1. Connect the heartbeat sensor to NodeMCU:
   - VCC to 3.3V
   - GND to GND
   - Signal to A0 (analog pin)

2. Flash the provided Arduino sketch via Arduino IDE.

3. Replace Wi-Fi credentials and API key placeholders with your actual details:
   ```cpp
   const char* ssid = "Your_WiFi_SSID";
   const char* password = "Your_WiFi_Password";
   const char* apiKey = "Your_ThingSpeak_API_Key";



