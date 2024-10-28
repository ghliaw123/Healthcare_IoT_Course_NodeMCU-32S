#define BRIGHT_MAX  1000

const int ledPin = 25;
const int lightPin = 36; 
void setup() {
  Serial.begin(115200);
  while(!Serial);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int value;

  value = analogRead(lightPin);
  Serial.println(value);
  analogWrite(ledPin, map(value, 4095, BRIGHT_MAX, 255, 0));
}
