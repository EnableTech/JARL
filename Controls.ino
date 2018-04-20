#include <Arduino.h>
#include "DRV8825.h"
#include <ams_as5048b.h>

// unit consts
#define U_RAW 1
#define U_DEG 3

// pins
#define SHOULDER_STEP 7
#define SHOULDER_DIR 6
#define ELBOW_STEP 5
#define ELBOW_DIR 4
#define MODE0 10
#define MODE1 9
#define MODE2 8
#define RIGHT 3
#define LEFT 2
#define FORWARD 1
#define BACK 0

// To change
#define DELTA 2         // delta x and delta y
#define OFFSET1 200      // absolute angle1
#define OFFSET2 180      // absolute angle2
#define RPM 60          // speed motors turn
#define LEN1 45         // arm length 1 (between shoulder and elbow)
#define LEN2 75         // arm length 2

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degees/step
#define MOTOR_STEPS 200
#define MICROSTEP 8

// Motors
DRV8825 stepper(MOTOR_STEPS, SHOULDER_DIR, SHOULDER_STEP, MODE0, MODE1, MODE2);
DRV8825 stepper2(MOTOR_STEPS, ELBOW_DIR, ELBOW_STEP, MODE0, MODE1, MODE2);

// Encoders
AMS_AS5048B mysensor(0x42);
AMS_AS5048B mysensor2(0x44);

// Temporary Variables
int r = 0;
int l = 0;
int f = 0;
int b = 0;
int theta1 = 0;
int theta2 = 0;
int x = 0;
int y = 0;

void setup() {
  Serial.begin(9600);

  // Set target motor RPM and MICROSTEP
  stepper.begin(RPM, MICROSTEP);
  stepper2.begin(RPM, MICROSTEP);

  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(FORWARD, INPUT);
  pinMode(BACK, INPUT);

  // init encoders
  mysensor.begin();
  mysensor2.begin();
}

void loop() {
  r = digitalRead(RIGHT);
  l = digitalRead(LEFT);
  f = digitalRead(FORWARD);
  b = digitalRead(BACK);
  theta1 = mysensor.angleR(U_DEG, false);
  theta2 = mysensor2.angleR(U_DEG, false);
  x = distX(theta1, theta2);
  y = distY(theta1, theta2);

  // Test input buttons
  Serial.println(r);
  Serial.println(l);
  Serial.println(f);
  Serial.println(b);

  // Test encoders
  Serial.print(theta1);
  Serial.print("  ||  ");
  Serial.println(theta2);

  if (r != l && f == b && f == LOW) {
    if (r == HIGH) {
      if (edge(x, y)) {
        stepper.rotate(0.25);
      } else {
        double* theta = angles(x + DELTA, y);
        stepper.rotate(theta[0]-theta1);
        stepper2.rotate((theta[0]+theta[1])-theta2);
      }
    } else {
      if (edge(x, y)) {
        stepper.rotate(-0.25);
      } else {
        double* theta = angles(x - DELTA, y);
        stepper.rotate(theta[0]-theta1);
        stepper2.rotate((theta[0]+theta[1])-theta2);
      }
    }
  }
  else if (f != b && r == l && r == LOW) {
    if (f == HIGH) {
      if (edge(x, y)) {
        if (abs(theta1 - 180) < 5) {
          stepper.rotate(0.25);
        } else {
          stepper.rotate(-0.25);
        }
      } else {
        double* theta = angles(x, y + DELTA);
        stepper.rotate(theta[0]-theta1);
        stepper2.rotate((theta[0]+theta[1])-theta2);
      }
    } else {
      if (edge(x, y)) {
        if (abs(theta1 - 180) < 5) {
          stepper.rotate(-0.25);
        } else {
          stepper.rotate(0.25);
        }
      } else {
        double* theta = angles(x, y - DELTA);
        stepper.rotate(theta[0]-theta1);
        stepper2.rotate((theta[0]+theta[1])-theta2);
      }
    }
  }

  delay(10);
}

// Helper functions
double getAngle(double x, double y) {
  return 0;
}

double distX(double theta1, double theta2) {
  return LEN1*cos(theta1) + LEN2*cos(theta2);
}

double distY(double theta1, double theta2) {
  return LEN1*sin(theta1) + LEN2*sin(theta2);
}

boolean edge(double x, double y) {
  if (abs(sqrt(pow(x,2) + pow(y,2))-(LEN1 + LEN2)) < 5)
    return true;
  else
    return false;
}

double* angles(double x, double y) {
  theta2 = 2*atan(sqrt((pow(LEN1+LEN2,2)-(pow(x,2)+pow(y,2)))/
           (pow(x,2)+pow(y,2)-pow(LEN1-LEN2,2))));
  if (x == 0)
    x = 0.1;
  theta1 = atan(y/x) - atan(LEN2*sin(theta2)/(LEN1+LEN2*cos(theta2)));
  double theta[] = {theta1, theta2};
  return theta;
}

