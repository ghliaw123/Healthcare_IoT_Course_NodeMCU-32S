/*
 * Toggle LED ON/OFF by Light degree (Interrupt)
 * (by modifying example "05-0-2_Interrupt")
*/
// Pin assignment
#define PIN_LIGHT 16
#define PIN_LED   LED_BUILTIN

// Debounce Delay
#define DEBOUNCE_DELAY  50  //ms

// Global Variables
unsigned long lastDebounceTime = 0;
uint8_t ledStatus;
//bool intOccur = false;

void setup() {
  //Serial.begin(9600);
  pinMode(PIN_LIGHT, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  if (digitalRead(PIN_LIGHT) == HIGH) ledStatus = HIGH;
  else ledStatus = LOW;
  digitalWrite(PIN_LED, ledStatus);
  attachInterrupt(PIN_LIGHT, isr, CHANGE);
}

void loop() {
  // for Debug
/*
  if (intOccur) {
    Serial.println("intr occur!");
    intOccur = false;
  }
*/
  if (lastDebounceTime > 0) { // light statue has ever been changed
    // Check if debounce delay is over
    if (millis()-lastDebounceTime > DEBOUNCE_DELAY) {
      if (digitalRead(PIN_LIGHT) == HIGH) ledStatus = HIGH;
      else ledStatus = LOW;
      digitalWrite(PIN_LED, ledStatus);
      lastDebounceTime = 0;
    }
  }
}

// ISR for button pin interrupt
void isr() {
  //intOccur = true;
  lastDebounceTime = millis();
}
