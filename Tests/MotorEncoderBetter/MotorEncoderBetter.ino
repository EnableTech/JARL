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

double upper = 4;
double lower = 4;
double u2 = upper * upper;
double l2 = lower * lower;
double ul2 = (upper + lower) * (upper + lower);

#define DIR0 3
#define STP0 4
#define DIR1 5
#define STP1 6
#define DIR2 7
#define STP2 8

#define B0 9
#define B1 10
#define B2 11
#define B3 12
#define B4 13
#define B5 2

DRV8825 stepper0(MOTOR_STEPS, DIR0, STP0);
DRV8825 stepper1(MOTOR_STEPS, DIR1, STP1);
DRV8825 stepper2(MOTOR_STEPS, DIR2, STP2);

#define PINS_LEN 6
int pins[] = {B0, B1, B2, B3, B4, B5};
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
  // Motor setup  
  stepper0.begin(RPM);
  stepper0.enable();
  stepper0.setMicrostep(MICROSTEPS);
  stepper1.begin(RPM);
  stepper1.enable();
  stepper1.setMicrostep(MICROSTEPS);
  stepper2.begin(RPM);
  stepper2.enable();
  stepper2.setMicrostep(MICROSTEPS);

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
        stepper0.rotate((toDeg(atan(y / x) - acos((u2 + (x * x) + (y * y) - l2) / (2 * upper * sqrt((x * x) + (y * y)))) - PI) - normDeg(angle0 - 180)));
        stepper1.rotate(-(toDeg(PI -acos((u2 + l2 - (x * x) - (y * y)) / (2 * upper * lower))) - normDeg(angle1)));
      }
    }
  }

  if (vals[4] == HIGH && vals[5] == LOW) stepper2.move(5);
  else if (vals[5] == HIGH) stepper2.move(-5);
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
  angle0 =  normDeg(encoder0.angleR(U_DEG, true) - 90);
  angle1 = normDeg(encoder1.angleR(U_DEG, true));
  curr_x = upper * cos(toRad(angle0)) + lower * cos(toRad(angle0 + angle1));
  curr_y = upper * sin(toRad(angle0)) + lower * sin(toRad(angle0 + angle1));
}
