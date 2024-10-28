#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definitions
#define DHTPIN 17     // pin for DHT Sensor
#define LED_TEMP  25  // pin for LED indicating over temperaure
#define LED_HUM   26  // pin for LED indicating over humidity
#define TEMP_LIMIT  30.0  // Temperature threshold
#define HUM_LIMIT   60.0  // Humidity Threshold
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C Address of OLED module

// Uncomment whatever DHT type you're using!
#define DHTTYPE DHT22   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Global variables
DHT dht(DHTPIN, DHTTYPE); // DHT object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
    // Initialize Serial port
    Serial.begin(115200); 

    // Set LED pins as output
    pinMode(LED_TEMP, OUTPUT);
    pinMode(LED_HUM, OUTPUT);

    // Initialize DHT
    dht.begin();

    // Initialize OLED display
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }
    // Clear the buffer OLED and do default setting
    display.clearDisplay(); 
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    Serial.println("Lab start!");
}

void loop() 
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    char buf[17];
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        // Print Temperature & Humidity data on Serial port
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");

        // Print Temperature & Humidity data on OLED
        char buf[20];
        display.clearDisplay();
        display.setCursor(0, 0);
        sprintf(buf, "Temp: %.1f\xF8\x43", t);
        display.println(buf);
        sprintf(buf, "Hum:  %.1f%%", h);
        display.println(buf);
        display.display();

        // if value > limit, LED on; else LED off
        if(t >= TEMP_LIMIT)
          digitalWrite(LED_TEMP, HIGH);
        else
          digitalWrite(LED_TEMP, LOW);
        if(h >= HUM_LIMIT)
          digitalWrite(LED_HUM, HIGH);
        else
          digitalWrite(LED_HUM, LOW);
    }
    delay(2000);
}
