#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <AESLib.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "****";                    // Replace with your WiFi SSID
const char* password = "****";           // Replace with your WiFi password

// ThingSpeak MQTT credentials
const char* mqttServer = "mqtt3.thingspeak.com";
const int mqttPort = 1883;                    // ThingSpeak MQTT standard port
const char* mqttClientID = "*****";  // Obtain from ThingSpeak MQTT devices
const char* mqttUser = "*****";      // Obtain from ThingSpeak MQTT devices
const char* mqttPassword = "*****"; // Obtain from ThingSpeak MQTT devices
const char* topicHeartRate = "****";  // Heart rate field
const char* topicSpO2 = "*****";      // SpO2 field

WiFiClient espClient;
PubSubClient mqttClient(espClient);
AESLib aesLib;

// MAX30100 sensor
PulseOximeter pox;

// AES Encryption key and IV (NZTA-inspired encryption)
byte aes_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x8f, 0x1e, 0xf4, 0x9b, 0x2b, 0x31};
byte aes_iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

// Timing variables
unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds delay

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  connectToWiFi();

  // Initialize MQTT
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setKeepAlive(60); 
  connectToMQTT();

  // Initialize MAX30100 sensor
  if (!pox.begin()) {
    Serial.println("MAX30100 initialization failed!");
    while (1);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); 
}

void loop() {
  // Ensure WiFi is connected
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Ensure MQTT client stays connected
  if (!mqttClient.connected()) {
    connectToMQTT();
  }

  mqttClient.loop();  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Update sensor values
    pox.update();
    float heartRate = pox.getHeartRate();
    float spO2 = pox.getSpO2();

    // Ensure valid sensor data is available
    if (heartRate > 0 && spO2 > 0) {
      // Encrypt and send data
      String encryptedHeartRate = encryptData(String(heartRate));
      String encryptedSpO2 = encryptData(String(spO2));

      mqttClient.publish(topicHeartRate, encryptedHeartRate.c_str());
      mqttClient.publish(topicSpO2, encryptedSpO2.c_str());

      Serial.print("Heart Rate: ");
      Serial.println(heartRate);
      Serial.print("SpO2: ");
      Serial.println(spO2);
    } else {
      Serial.println("Failed to read sensor data.");
    }
  }
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi.");
}

// Function to connect to MQTT server
void connectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqttClient.connect(mqttClientID, mqttUser, mqttPassword)) {
      Serial.println("MQTT connected.");
    } else {
      Serial.print("Failed to connect, retrying in 5 seconds. Error: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

// AES encryption function
String encryptData(String data) {
  byte plain_data[32];
  data.getBytes(plain_data, sizeof(plain_data));

  char encrypted_data[128]; 
  aesLib.encrypt64(plain_data, sizeof(plain_data), encrypted_data, aes_key, 128, aes_iv); 

  return String(encrypted_data);
}  
