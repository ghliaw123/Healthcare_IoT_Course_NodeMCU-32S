#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Your SSID";  // Enter SSID here
const char* password = "Your Password";  //Enter Password here

/* Construct a WebServer Object */
WebServer server(80);

/* Define pins and statuses of LEDs */
uint8_t redPin = 25;
bool redStatus = LOW;
uint8_t greenPin = 27;
bool greenStatus = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  /* Connect to Wi-Fi network */
  Serial.println();
  Serial.print("Connect to Wi-Fi network: ");
  Serial.println("ssid");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  /* Set the correspondence between URL and its handler */
  server.on("/", handle_OnConnect);
  server.on("/red_on", handle_red_on);
  server.on("/red_off", handle_red_off);
  server.on("/green_on", handle_green_on);
  server.on("/green_off", handle_green_off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  digitalWrite(redPin, redStatus);
  digitalWrite(greenPin, greenStatus);
}

void handle_OnConnect() {
  redStatus = LOW;
  greenStatus = LOW;
  Serial.println("GPIO25 Status: OFF | GPIO27 Status: OFF");
  server.send(200, "text/html", SendHTML(redStatus,greenStatus)); 
}

void handle_red_on() {
  redStatus = HIGH;
  Serial.println("GPIO25 Status: ON");
  server.send(200, "text/html", SendHTML(true,greenStatus)); 
}

void handle_red_off() {
  redStatus = LOW;
  Serial.println("GPIO25 Status: OFF");
  server.send(200, "text/html", SendHTML(false,greenStatus)); 
}

void handle_green_on() {
  greenStatus = HIGH;
  Serial.println("GPIO27 Status: ON");
  server.send(200, "text/html", SendHTML(redStatus,true)); 
}

void handle_green_off() {
  greenStatus = LOW;
  Serial.println("GPIO27 Status: OFF");
  server.send(200, "text/html", SendHTML(redStatus,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>Red LED Status: ON</p><a class=\"button button-off\" href=\"/red_off\">OFF</a>\n";}
  else
  {ptr +="<p>Red LED Status: OFF</p><a class=\"button button-on\" href=\"/red_on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>Green LED Status: ON</p><a class=\"button button-off\" href=\"/green_off\">OFF</a>\n";}
  else
  {ptr +="<p>Green LED Status: OFF</p><a class=\"button button-on\" href=\"/green_on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}