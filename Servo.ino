#include <Servo.h>

Servo xServo;
Servo yServo;
int light = 7; // connect first relay1 to pin 7----light
int pump = 8; // connect second relay2 to pin 8---pump
int smokePin = 11; // connect MQ2 smoke detector to analog pin A0
int buzzer = 6;
void setup() {
  xServo.attach(9);  // connect x servo to pin 9
  yServo.attach(10); // connect y servo to pin 10
  pinMode(light, OUTPUT); // set first relay pin as output
  pinMode(pump, OUTPUT); // set second relay pin as output
  pinMode(buzzer, OUTPUT); // set second relay pin as output
  pinMode(smokePin, INPUT); // set smoke detector pin as input
  digitalWrite(light, LOW); // turn off first relay initially
  digitalWrite(pump, HIGH); // turn off second relay initially
  Serial.begin(9600); // initialize serial communication
  xServo.write(0);
  yServo.write(0);
}

void loop() {
  if (Serial.available() > 0) { // if there's data available to read

    int xCoord = Serial.parseInt(); // read x coordinate
    int yCoord = Serial.parseInt(); // read y coordinate
    if (Serial.read() == '\n') { // check if end of line is received
      // map x and y coordinates from 0-180 range to servo angle range (0-180)
      int xAngle = map(xCoord, 0, 30, 70, 0);
      int yAngle = map(yCoord, 0, 30, 70, 0);
      // turn off first relay
      digitalWrite(light, LOW);
      digitalWrite(buzzer, HIGH);
      delay(2000);
      // move servos to new angles
      xServo.write(xAngle);
      yServo.write(yAngle);
      delay(2000);
      digitalWrite(pump, LOW);

      // wait for servos to reach new angles
      delay(3000);
      digitalWrite(buzzer, LOW);
      digitalWrite(light, HIGH);
      digitalWrite(pump, HIGH);
      //delay(3000);
      
      
      
    }
  } 
  else {
    // turn off both relays if no data is received for 1 second
    digitalWrite(light, HIGH);
    digitalWrite(pump, HIGH);
    delay(1000);
  }
  // check for smoke
  int smokeLevel = digitalRead(smokePin);
  if (smokeLevel == 0) { // adjust threshold based on your smoke detector
    // turn off both relays
    digitalWrite(buzzer, HIGH);
     
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}
