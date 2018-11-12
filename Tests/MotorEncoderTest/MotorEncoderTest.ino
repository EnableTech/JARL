#include <Arduino.h>
#include "DRV8825.h"

#include <ams_as5048b.h>
#define U_RAW 1
#define U_DEG 3

AMS_AS5048B mysensor(0x42);
AMS_AS5048B mysensor2(0x44);

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120
#define MICROSTEPS 32

double upper = 6;
double lower = 6;
double u2 = upper * upper;
double l2 = lower * lower;

#define DIR1 3
#define STP1 4
#define DIR2 5
#define STP2 6
#define DIR3 7
#define STP3 8

#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13
#define B6 2

DRV8825 stepper1(MOTOR_STEPS, DIR1, STP1);
DRV8825 stepper2(MOTOR_STEPS, DIR2, STP2);
DRV8825 stepper3(MOTOR_STEPS, DIR3, STP3);

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  stepper1.begin(RPM);
  stepper1.enable();
  stepper1.setMicrostep(MICROSTEPS);
  stepper2.begin(RPM);
  stepper2.enable();
  stepper2.setMicrostep(MICROSTEPS);
  stepper3.begin(RPM);
  stepper3.enable();
  stepper3.setMicrostep(MICROSTEPS);
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(B3, INPUT);
  pinMode(B4, INPUT);
  pinMode(B5, INPUT);
  pinMode(B6, INPUT);

  mysensor.begin();
  mysensor2.begin();
}

void loop() {
  int v1 = digitalRead(B1);
  int v2 = digitalRead(B2);
  int v3 = digitalRead(B3);
  int v4 = digitalRead(B4);
  int v5 = digitalRead(B5);
  int v6 = digitalRead(B6);

  //if (v1 || v2 || v3 || v4) {
    mysensor.angleR(U_RAW, true);
    double d1 = normDeg(mysensor.angleR(U_DEG, false) - 90);
    mysensor2.angleR(U_RAW, true);
    double d2 = normDeg(mysensor2.angleR(U_DEG, false));
    Serial.print(d1);
    Serial.print("\t\t");
    Serial.println(d2);
    /*
    double r1 = toRad(d1);
    double r2 = toRad(d2);
    double x = upper * cos(r1) + lower * cos(r1 + r2) + v1 * (0.5) + v2 * (-0.5);
    double y = upper * sin(r1) + lower * sin(r1 + r2) + v3 * (0.5) + v4 * (-0.5);
    stepper1.rotate(angle1(x, y) - d1);
    stepper2.rotate(angle2(x, y) - d2);
    */
  //}
/*
  if (v5 == HIGH && v6 == LOW) {
    stepper3.move(5);
  }
  else if (v6 == HIGH) {
    stepper3.move(-5);
  }
*/
  delayMicroseconds(1);
  delay(1000);
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
  if (d < 0) {
    return normDeg(d + 360);
  }
  else if (d >= 360) {
    return normDeg(d - 360);
  }
  return d;
}

// Returns the intended angle 1 in degrees given intended x and y
double angle1 (double x, double y) {
  return toDeg(atan(y / x) - acos((u2 + (x * x) + (y * y) - l2) / (2 * upper * sqrt((x * x) + (y * y)))));
}

// Returns the intended angle 2 in degrees given intended x and y
double angle2 (double x, double y) {
  return toDeg(PI - acos((u2 + l2 - (x * x) - (y * y)) / (2 * upper * lower)));
}
