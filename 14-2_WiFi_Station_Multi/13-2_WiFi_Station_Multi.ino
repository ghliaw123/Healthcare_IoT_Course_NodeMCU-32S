#include <WiFi.h>
#include <WiFiMulti.h>

// Global variables
static const char* ssid1 = "31N";  // Change this to your desired WiFi1 SSID
static const char* password1 = "087389887";  // Change this to your desired WiFi1 password
static const char* ssid2 = "Liaw";  // Change this to your desired WiFi2 SSID
static const char* password2 = "12345678";  // Change this to your desired WiFi2 password
WiFiMulti wmulti;

void setup() {
  // Initialize serial port
  Serial.begin(115200);

  // Setup desired WiFi network
  Serial.print("Desired Wi-Fi Network: ");
  Serial.println(String(ssid1) + String(" ,") + String(ssid2));
  wmulti.addAP(ssid1, password1);
  wmulti.addAP(ssid2, password2);

  // Connect to one of the desired WiFi network
  while (wmulti.run()!=WL_CONNECTED) {
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
