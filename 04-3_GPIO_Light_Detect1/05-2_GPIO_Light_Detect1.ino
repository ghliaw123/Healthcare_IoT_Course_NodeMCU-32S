/*
 * Toggle LED ON/OFF by Light degree (Polling)
 * (by modifying example "Debounce")
*/
// constants won't change. They're used here to set pin numbers:
const int lightPin = 16;  // the number of the light sensor DO pin
const int ledPin = LED_BUILTIN;    // the number of the LED pin

// Variables will change:
int ledState = LOW;        // the current state of the output pin
int lightState;            // the current reading from the input pin
int lastLightState = HIGH;  // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  //Serial.begin(9600);
  pinMode(lightPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of light sensor into a local variable:
  int reading = digitalRead(lightPin);
  //Serial.println(String("Reading=")+String(reading));

  // check to see if the light state just was changed
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last change to ignore any noise:

  // If the state changed, due to noise or pressing:
  if (reading != lastLightState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != lightState) {
      lightState = reading;

      // Set LED state according to light sensor state
      if (lightState == HIGH)  ledState = HIGH;
      else ledState = LOW;
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastLightState = reading;
}
