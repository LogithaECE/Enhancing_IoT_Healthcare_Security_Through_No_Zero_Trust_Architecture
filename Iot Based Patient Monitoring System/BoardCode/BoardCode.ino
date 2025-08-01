#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// WiFi credentials
const char* ssid = "*****";            // Replace with your WiFi SSID
const char* password = "*****";   // Replace with your WiFi password

// ThingSpeak details
unsigned long channelID = ####;    // Replace with your ThingSpeak channel ID
const char* apiKey = "*****";  // Replace with your ThingSpeak API Key

WiFiClient client;

unsigned long previousMillis = 0;
const long interval = 20000; // 10 seconds delay

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  unsigned long currentMillis = millis();

  // Send data every 10 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Generate random values within a reasonable range
    float heartRate = random(70, 75);  // Random heart rate between 60 and 100
    float spO2 = random(95, 98);       // Random SpO2 between 90 and 100

    // Print values to the Serial Monitor
    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm / SpO2: ");
    Serial.print(spO2);
    Serial.println(" %");

    // Send data to ThingSpeak
    ThingSpeak.setField(1, heartRate);  // Field 1 is for Heart Rate
    ThingSpeak.setField(2, spO2);       // Field 2 is for SpO2

    int status = ThingSpeak.writeFields(channelID, apiKey);

    if (status == 200) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.println("Error sending data: " + String(status));
    }
  }
}
