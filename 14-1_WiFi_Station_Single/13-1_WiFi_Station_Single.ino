#include <WiFi.h>

// Global variables
static const char* ssid = "your-ssid";  // Change this to your WiFi SSID
static const char* password = "your-password";  // Change this to your WiFi password

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.print("Connecting to Wi-Fi Network: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // After successfully connected, output some info
  Serial.println();
  Serial.println("Wi-Fi Connected!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("AP's MAC Address: ");
  Serial.println(WiFi.BSSIDstr());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  Serial.print("My MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("My IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway (default Router): ");
  Serial.println(WiFi.gatewayIP());
}

void loop() {
}
