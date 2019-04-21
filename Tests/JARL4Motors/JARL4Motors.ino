//130
#include <Arduino.h>
#include "DRV8825.h"

#include <ams_as5048b.h>
#define U_RAW 1
#define U_DEG 3

AMS_AS5048B encoder0(0x42);
AMS_AS5048B encoder1(0x44);

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120
#define MICROSTEPS 32

const double upper = 10.9;
const double lower = 8.3;
const double u2 = upper * upper;
const double l2 = lower * lower;
const double ul2 = (upper + lower) * (upper + lower);

#define LINEAR_DIR 2
#define LINEAR_STP 3
#define SHOULDER_DIR 4
#define SHOULDER_STP 5
#define ELBOW_DIR 6
#define ELBOW_STP 7
#define GRIPPER_DIR 8
#define GRIPPER_STP 9

#define BACK 10
#define FORWARD 11
#define LEFT 12
#define RIGHT 13
#define CLOSE A1
#define OPEN A0
#define DOWN A3
#define UP A2

DRV8825 linear(MOTOR_STEPS, LINEAR_DIR, LINEAR_STP);
DRV8825 shoulder(MOTOR_STEPS, SHOULDER_DIR, SHOULDER_STP);
DRV8825 elbow(MOTOR_STEPS, ELBOW_DIR, ELBOW_STP);
DRV8825 gripper(MOTOR_STEPS, GRIPPER_DIR, GRIPPER_STP);

#define PINS_LEN 8
int pins[] = {BACK, FORWARD, LEFT, RIGHT, CLOSE, OPEN, DOWN, UP};
int vals[PINS_LEN];
double x;
double y;
double angle0;
double angle1;
double temp_x;
double temp_y;
double curr_x;
double curr_y;

void setup() {
        Serial.begin(9600);
  // Motor setup  
  setupMotor(linear);
  setupMotor(shoulder);
  setupMotor(elbow);
  setupMotor(gripper);

  // Input pin setup
  for (int i = 0; i < PINS_LEN; i++) pinMode(pins[i], INPUT);

  // Encoder setup
  encoder0.begin();
  encoder1.begin();

  compute();
  x = curr_x;
  y = curr_y;
}

void loop() {
  // Read input pins
  for (int i = 0; i < PINS_LEN; i++) vals[i] = digitalRead(pins[i]);

  if (vals[0] || vals[1] || vals[2] || vals[3]) {
    compute();
    temp_x = x + vals[0] * (0.1) + vals[1] * (-0.1);
    temp_y = y + vals[2] * (0.1) + vals[3] * (-0.1);
    if (temp_x * temp_x + temp_y * temp_y <= ul2) {
      x = temp_x;
      y = temp_y;
      if(abs(x - curr_x) > 0.01 || abs(y - curr_y) > 0.01){
        shoulder.rotate(-((toDeg(atan(y / x) - acos((u2 + (x * x) + (y * y) - l2) / (2 * upper * sqrt((x * x) + (y * y)))) - PI) - normDeg(angle0))));
        elbow.rotate(-(-(toDeg(-acos((u2 + l2 - (x * x) - (y * y)) / (2 * upper * lower))) - normDeg(angle1 - 180 - 90))));
      }
    }
  }

  if (vals[4] == HIGH && vals[5] == LOW) gripper.move(5);
  else if (vals[5] == HIGH) gripper.move(-5);
  if (vals[6] == HIGH && vals[7] == LOW) linear.move(5);
  else if (vals[7] == HIGH) linear.move(-5);
  delayMicroseconds(1);
}

// Converts an angle in degrees to radians (with normalization)
double toRad(double d) {
  return normDeg(d) * 2 * PI / 360;
}

// Converts an angle in radians to degrees (with normalization)
double toDeg(double d) {
  return normDeg(d * 360 / (2 * PI));
}

// Normalizes an angle in degrees to one where 0 <= theta < 360
double normDeg(double d) {
  if (d < 0) return normDeg(d + 360);
  else if (d >= 360) normDeg(d - 360);
  return d;
}

// Reads both encoders and assigns values into angle0 and angle1
// Uses encoder values to calculate current x and y and assigns values into curr_x and curr_y
double compute(){
  angle0 = normDeg(encoder0.angleR(U_DEG, true) - 90 - 180);
  angle1 = normDeg(encoder1.angleR(U_DEG, true) - 90);
  Serial.println(angle0);
  Serial.println(angle1);
  Serial.println();
  curr_x = upper * cos(toRad(angle0)) + lower * cos(toRad(angle0 + angle1));
  curr_y = upper * sin(toRad(angle0)) + lower * sin(toRad(angle0 + angle1));
}

void setupMotor(DRV8825 motor){
  motor.begin(RPM);
  motor.enable();
  motor.setMicrostep(MICROSTEPS);
}

