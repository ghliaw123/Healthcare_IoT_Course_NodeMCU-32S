/*
    Go to thingspeak.com and create an account if you don't have one already.
    After logging in, click on the "New Channel" button to create a new channel for your data. This is where your data will be stored and displayed.
    Fill in the Name, Description, and other fields for your channel as desired, then click the "Save Channel" button.
    Take note of the "Write API Key" located in the "API keys" tab, this is the key you will use to send data to your channel.
    Replace the channelID from tab "Channel Settings" and privateKey with "Read API Keys" from "API Keys" tab.
    Replace the host variable with the thingspeak server hostname "api.thingspeak.com"
    Upload the sketch to your ESP32 board and make sure that the board is connected to the internet. The ESP32 should now send data to your Thingspeak channel at the intervals specified by the loop function.
    Go to the channel view page on thingspeak and check the "Field1" for the new incoming data.
    You can use the data visualization and analysis tools provided by Thingspeak to display and process your data in various ways.
    Please note, that Thingspeak accepts only integer values.

    You can later check the values at https://thingspeak.com/channels/2005329
    Please note that this public channel can be accessed by anyone and it is possible that more people will write their values.
 */

#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

// Definitions
#define DHTPIN 17
#define DHTTYPE DHT22 

// Global variables:
// WiFi network parameters
const char* ssid     = "your-ssid"; // Change this to your WiFi SSID
const char* password = "your-password"; // Change this to your WiFi password

// ThingSpeak channel parameters
const char* host = "api.thingspeak.com"; // This should not be changed
const int httpPort = 80; // This should not be changed
const String channelID   = "2005329"; // Change this to your channel ID
const String writeApiKey = "V6YOTILH9I7D51F9"; // Change this to your Write API key
const String readApiKey = "34W6LGLIFXD56MPM"; // Change this to your Read API key

// DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Parameters for reading data from ThingSpeak Channel
int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out

void setup()
{
    // Initialize serial port
    Serial.begin(115200);
    while(!Serial){delay(100);}

    // Connecting to a WiFi network
    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize DHT Sensor
    dht.begin();
}

void loop(){
  HTTPClient http;
  String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n" + "Connection: close\r\n\r\n";
  
  // Get Temperature & Humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // WRITE --------------------------------------------------------------------------------------------
  String url = String("http://") 
                + String(host) 
                + String("/update?api_key=") 
                + String(writeApiKey) 
                + String("&field1=") + String(t) 
                + String("&field2=") + String(h);
  http.begin(url);
  int httpcode = http.GET();
  Serial.println("Upload data ....");
  if (httpcode>0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpcode);
    if (httpcode==HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpcode).c_str());
  }
  http.end();

  // READ --------------------------------------------------------------------------------------------
  url = String("http://") 
              + String(host) 
              + String("/channels/") + String(channelID) 
              + String("/fields/") + String(fieldNumber) 
              + String(".json?results=") + String(numberOfResults);
  http.begin(url);
  httpcode = http.GET();
  Serial.println("Download data ....");
  if (httpcode>0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpcode);
    if (httpcode==HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpcode).c_str());
  }
  http.end(); 

  // -------------------------------------------------------------------------------------------------
  delay(15000);
}