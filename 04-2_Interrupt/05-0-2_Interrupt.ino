// Pin assignment
#define PIN_BUTTON  17
#define PIN_LED     25

// Debounce Delay
#define DEBOUNCE_DELAY  50  //ms

// Global Variables
unsigned long lastDebounceTime = 0;
uint8_t ledStatus = HIGH;

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLDOWN);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, ledStatus);
  attachInterrupt(PIN_BUTTON, isr, RISING);
}

void loop() {
  if (lastDebounceTime > 0) { // button has ever been pressed
    // Check if debounce delay is over
    if (millis()-lastDebounceTime > DEBOUNCE_DELAY) {
      if (digitalRead(PIN_BUTTON) == HIGH) {
        ledStatus = !ledStatus;
        digitalWrite(PIN_LED, ledStatus);
      }
      lastDebounceTime = 0;
    }
  }
}

// ISR for button pin interrupt
void isr() {
  lastDebounceTime = millis();
}
