#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potPin = 36;  // analog pin used to connect the potentiometer
int servoPin = 25;
int val;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(115200);
  while(!Serial);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 2047, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  Serial.println(val);
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}
