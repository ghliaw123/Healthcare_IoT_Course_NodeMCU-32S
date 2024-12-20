#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Definitions and declarations for DHT sensor
#define DHTPIN 17 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

// WiFi AP ssid / password here
static const char* ssid = "your-ssid";  // Change this to your WiFi SSID
static const char* password = "your-password";  // Change this to your WiFi password

// MQTT Broker info
//IPAddress server(140, 127, 196, 119);
static char *server = "test.mosquitto.org";
int port = 1883;

// MQTT topics
#define TOPIC_INFO  "xxxxxxxx/info"
#define TOPIC_TEMP  "xxxxxxxx/temp"
#define TOPIC_HUM   "xxxxxxxx/hum"
#define TOPIC_LED_CONTROL "xxxxxxxx/led_control"

// MQTT Client info
// Client ID.
// Note that a broker allows an individual client to create only on session.
// If a session is created by another client with same cliend ID, the former one will be disconnected.
// Thus, each sensor node's client ID must be different from each other. 
static char *client_id = "SensorNode_001"; 

// Clients for MQTT
WiFiClient wifiClient;
PubSubClient client(wifiClient);

// LED Control info
#define LED_PIN LED_BUILTIN
typedef enum {
  LED_OFF = 0,
  LED_ON,
  LED_FLASH
} LEDStatus;
LEDStatus led_status = LED_OFF;

// Timer info
#define TEMP_PERIOD 10000
#define LED_FLASH_PERIOD 200
unsigned long temp_last_time, led_last_time;

void led_on(){
  digitalWrite(LED_PIN, HIGH);
}

void led_off(){
  digitalWrite(LED_PIN, LOW);
}

void led_toggle() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Output incoming message to serial terminal
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // If LED Control command is incoming, change LED status
  if(!strcmp(topic, TOPIC_LED_CONTROL)) {
    switch (payload[0]) {
      case '0':
        led_status = LED_OFF;
        break;
      case '1':
        led_status = LED_ON;
        break;
      case '2':
        led_status = LED_FLASH;
        break;
      default: {}
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC_INFO,"sensor node ready ...");
      // ... and resubscribe
      client.subscribe(TOPIC_LED_CONTROL);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // setup Serial boudrate
  Serial.begin(115200);

  // Setup MQTT Client
  client.setServer(server, port); // Setup Server address and port
  client.setCallback(callback); // Setup callback function

  // start up DHT sensor
  dht.begin();

  // setup LED/Button pin
  pinMode(LED_PIN, OUTPUT);
  led_off();
  
  // setup Wifi connection
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  // Initialize variables for timing
  led_last_time = millis();
  temp_last_time = millis();
}

void loop() {
  unsigned long current_time;

  // Check MQTT broker connection status
  // If it is disconnected, reconnect to the broker
  if (!client.connected()) {
    reconnect();
  }

  // Get temperature & humidity and publish them
  current_time = millis();
  if( TEMP_PERIOD < (current_time - temp_last_time) ) {
    // Read Humidity & Temperature (Celsius)
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // Output to serial terminal
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
    // pubilsh to MQTT broker
    char buf[10];
    sprintf(buf,"%s", String((float)t, 2).c_str());
    client.publish(TOPIC_TEMP, buf);
    sprintf(buf,"%s", String((float)h, 2).c_str());
    client.publish(TOPIC_HUM, buf);
    // update last time value
    temp_last_time = current_time;
  }
  
  // Control LED according to led_status
  switch (led_status) {
    case LED_OFF:
      led_off();
      break;
    case LED_ON:
      led_on();
      break;
    case LED_FLASH:
      current_time = millis();
      if (LED_FLASH_PERIOD < (current_time - led_last_time)) {
        led_toggle();
        led_last_time = current_time;
      }
      break;
    default: {}
  }
  
  // Keep MQTT process on going
  client.loop();
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
