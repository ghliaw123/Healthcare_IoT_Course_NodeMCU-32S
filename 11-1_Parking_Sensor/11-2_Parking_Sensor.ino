#include <HCSR04.h>

// Definitions
#define CURRENT_TEMPERATURE 26.0
#define PIN_TRIG  16      // Trig Pin
#define PIN_ECHO  17      // Echo Pin
#define PIN_BUZZER  26    // Buzzer pin 
#define DISTANCE_FAR  30  // cm 
#define DISTANCE_NEAR 10  // cm
#define BUZZER_PERIOD_FAR     1000  // ms
#define BUZZER_PERIOD_MEDIUM  500  // ms
#define BUZZER_FREQ     1000 // Hz
#define BUZZER_DURATION 100 // ms

// Global variables
unsigned long last_time;
 
void setup() {
  // Initialize serial port
  Serial.begin (115200);           
  Serial.println("Program start.");
  // Initialize ultrasonic ranger
  HCSR04.begin(PIN_TRIG, PIN_ECHO);
  // Record the start time for timing
  last_time = millis();
}
 
void loop()
{
  unsigned long current_time, period;

  // Get distance
  double* d = HCSR04.measureDistanceCm(CURRENT_TEMPERATURE);
  double distance = d[0];
  Serial.print("Distance = ");
  Serial.println(distance);
  if (distance < 0) return;
  
  // According to distant range, play buzzer sound with corresponding period
  if (distance > DISTANCE_FAR)
    period = BUZZER_PERIOD_FAR;
  else if (distance < DISTANCE_NEAR)
    period = 0;
  else
    period = BUZZER_PERIOD_MEDIUM;

  if (period > 0) {
    current_time = millis();
    if (current_time - last_time >= period) {
      noTone(PIN_BUZZER);
      tone(PIN_BUZZER, BUZZER_FREQ, BUZZER_DURATION);
      last_time = current_time;
    }
  }
  else {
    tone(PIN_BUZZER, BUZZER_FREQ);
  }
  delay(100);
}
