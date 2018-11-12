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

  mysensor.angleR(U_RAW, true);
  double d1 = normDeg(mysensor.angleR(U_DEG, false) - 90);
  mysensor2.angleR(U_RAW, true);
  double d2 = normDeg(mysensor2.angleR(U_DEG, false));
  Serial.print(d1);
  Serial.print("\t\t");
  Serial.println(d2);
  double e1 = toRad(d1);
  double e2 = toRad(d2);
  double ax = upper * cos(e1) + lower * cos(e1 + e2);
  double ay = upper * sin(e1) + lower * sin(e1 + e2);
  /*
  if (v1 == HIGH && v2 == LOW) {
    double ix = ax;
    double iy = ay + 0.5;
    double a1 = atan(iy/ix) - acos((u2 + (ix * ix) + (iy * iy) - l2) / (2 * upper * sqrt((ix * ix) + (iy * iy))));
    double a2 = PI - acos((u2 + l2 - (ix * ix) - (iy * iy)) / (2 * upper * lower));
    stepper1.rotate(toDeg(a1) - d1);
    stepper2.rotate(toDeg(a2) - d2);
  }
  if (v2 == HIGH && v1 == LOW) {
    double ix = ax;
    double iy = ay - 0.5;
    double a1 = atan(iy/ix) - acos((u2 + (ix * ix) + (iy * iy) - l2) / (2 * upper * sqrt((ix * ix) + (iy * iy))));
    double a2 = PI - acos((u2 + l2 - (ix * ix) - (iy * iy)) / (2 * upper * lower));
    stepper1.rotate(toDeg(a1) - d1);
    stepper2.rotate(toDeg(a2) - d2);
  }

  if (v3 == HIGH && v4 == LOW) {
    double ix = ax + 0.5;
    double iy = ay;
    double a1 = atan(iy/ix) - acos((u2 + (ix * ix) + (iy * iy) - l2) / (2 * upper * sqrt((ix * ix) + (iy * iy))));
    double a2 = PI - acos((u2 + l2 - (ix * ix) - (iy * iy)) / (2 * upper * lower));
    stepper1.rotate(toDeg(a1) - d1);
    stepper2.rotate(toDeg(a2) - d2);
  }
  if (v4 == HIGH && v3 == LOW) {
    double ix = ax + 0.5;
    double iy = ay;
    double a1 = atan(iy/ix) - acos((u2 + (ix * ix) + (iy * iy) - l2) / (2 * upper * sqrt((ix * ix) + (iy * iy))));
    double a2 = PI - acos((u2 + l2 - (ix * ix) - (iy * iy)) / (2 * upper * lower));
    stepper1.rotate(toDeg(a1) - d1);
    stepper2.rotate(toDeg(a2) - d2);
  }

  if (v5 == HIGH && v6 == LOW) {
    stepper3.move(5);
  }
  if (v6 == HIGH && v5 == LOW) {
    stepper3.move(-5);
  }
*/
  delayMicroseconds(1);
  delay(1000);
}

double toRad(double d){
  return d * 2 * PI / 360;
}

double toDeg(double d){
  return normDeg(d * 360 / (2 * PI));
}

double normDeg(double d){
  if(d < 0){
    return normDeg(d + 360);
  }
  else if(d >= 360){
    return normDeg(d - 360);
  }
  return d;
}

