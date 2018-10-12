#include <Servo.h>

//Defines
#define PIN 10        //Linear Actuator Digital Pin

//max/min pulse values in microseconds for the linear actuators
#define MIN  1000     //
#define MAX  2000    //

Servo LIN;  // create servo objects to control the linear actuators
int val = 1500;  //current positional value being sent to the linear actuator.
boolean increase = true;

void setup() {
  LIN.attach(PIN, MIN, MAX);
  LIN.writeMicroseconds(val);
  Serial.begin(9600);
}

void loop() {
  if (increase) {
    val++;
  } else {
    val--;
  }
  if (val <= MIN) {
    increase = true;
  }
  if (val >= MAX) {
    increase = false;
  }
  Serial.println(val);
  LIN.writeMicroseconds(val);
  //delay(10);
}
