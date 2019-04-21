#include <Arduino.h>
#include "DRV8825.h"
#include <Wire.h>

// I2C specs
#define PAYLOAD_SIZE 1 // how many bytes to expect from each I2C slave node
#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes

// Dimensions in inches of upper arm, forearm, wrist
#define U 6
#define F 4
#define W 2
const double ufw2 = (U + F + W) * (U + F + W);

// Movement vars
#define SMP_ANG PI
#define INCR 0.5

// Motor specs
#define MOTOR_STEPS 200
#define RPM 2000
#define MICROSTEPS 32
#define GEAR_RATIO 51

// Motor pins for base, shoulder, elbow, wrist
#define BASE_DIR 2
#define BASE_STP 3
#define SHLD_DIR 4
#define SHLD_STP 5
#define ELBW_DIR 6
#define ELBW_STP 7
#define WRST_DIR 8
#define WRST_STP 9

// Motor control
#define MODE0 10
#define MODE1 11
#define MODE2 12
#define ENABLE 13

// Motors for base, shoulder, elbow, wrist
DRV8825 baseMotor(MOTOR_STEPS, BASE_DIR, BASE_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 shldMotor(MOTOR_STEPS, SHLD_DIR, SHLD_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 elbwMotor(MOTOR_STEPS, ELBW_DIR, ELBW_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 wrstMotor(MOTOR_STEPS, WRST_DIR, WRST_STP, ENABLE, MODE0, MODE1, MODE2);

// I2C payload
byte inputVals;

// Directions
bool rght = false;
bool left = false;
bool fwrd = false;
bool back = false;
bool upwd = false;
bool down = false;
bool dsbl = false;
bool temp = false;

// Positions of x, y, z coordinates and (initial) base, shoulder, elbow, wrist angles
double x = 0;
double y = 0;
double z = 0;
double b = 0;
double s = PI / 2;
double e = 0;
double w = 0;

// Temp vars
double r = 0;
double temp_x = 0;
double temp_y = 0;
double temp_z = 0;
double calc_x = 0;
double calc_y = 0;
double calc_z = 0;
double calc_b = 0;
double calc_s = 0;
double calc_e = 0;
double calc_w = 0;


void setup() {
  // I2C setup
  Serial.begin(9600);
  Serial.println("MASTER");
  Wire.begin();

  // Motor setup
  setupMotor(baseMotor);
  setupMotor(shldMotor);
  setupMotor(elbwMotor);
  setupMotor(wrstMotor);

  // Get initial positions
  calcXYZ(b, s, e);
  x = calc_x;
  y = calc_y;
  z = calc_z;
}

void loop() {
  Wire.requestFrom(START_NODE, PAYLOAD_SIZE);    // request data from node
  if (Wire.available() == PAYLOAD_SIZE) { // if data size is avaliable from nodes
    inputVals = Wire.read();  // get nodes data
    Serial.println(inputVals);
    processInputVals();
    simpleTurns(); // just for testing
    //moveToPos();
  }
  delay(1);
}

void setupMotor(DRV8825 motor) {
  motor.begin(RPM);
  motor.enable();
  motor.setMicrostep(MICROSTEPS);
}

void processInputVals() {
  rght = (bitRead(inputVals, 7) == 1);
  left = (bitRead(inputVals, 6) == 1);
  fwrd = (bitRead(inputVals, 5) == 1);
  back = (bitRead(inputVals, 4) == 1);
  upwd = (bitRead(inputVals, 3) == 1);
  down = (bitRead(inputVals, 2) == 1);
  dsbl = (bitRead(inputVals, 1) == 1);
  temp = (bitRead(inputVals, 0) == 1);
  if (dsbl) {
    pinMode(ENABLE, OUTPUT);
    digitalWrite(ENABLE, HIGH);
  }
}

void rotate(DRV8825 motor, double angle) {
  motor.rotate((int)(GEAR_RATIO * angle * 360 / (2 * PI)));
}

void simpleTurns() {
  if (rght) {
    rotate(baseMotor, SMP_ANG);
  }
  if (left) {
    rotate(baseMotor, -SMP_ANG);
  }
  if (fwrd) {
    rotate(shldMotor, SMP_ANG);
  }
  if (back) {
    rotate(shldMotor, -SMP_ANG);
  }
  if (upwd) {
    rotate(elbwMotor, SMP_ANG);
  }
  if (down) {
    rotate(elbwMotor, -SMP_ANG);
  }
}

void moveToPos() {
  temp_x = x + rght - left;
  temp_y = y + fwrd - back;
  temp_z = z + upwd - down;
  if ((temp_x + temp_y + temp_z) * (temp_x + temp_y + temp_z) <= ufw2) {
    x = temp_x;
    y = temp_y;
    z = temp_z;
    calcBSE(x, y, z);
    rotate(baseMotor, calc_b - b);
    rotate(shldMotor, calc_s - s);
    rotate(elbwMotor, calc_e - e);
    b = calc_b;
    s = calc_s;
    e = calc_e;
  }
}

void calcXYZ(int calc_b, int calc_s, int calc_e) {
  r = U * cos(calc_s) + F * cos(calc_e + calc_s - PI);
  calc_x = sqrt(r * r + W * W) * cos(calc_b - atan(W / r));
  calc_y = sqrt(r * r + W * W) * sin(calc_b - atan(W / r));
  calc_z = U * sin(calc_s) + F * sin(calc_e + calc_s - PI);
}

void calcBSE(int calc_x, int calc_y, int calc_z) {
  r = sqrt(calc_x * calc_x + calc_y * calc_y - W * W);
  calc_b = atan(W / r) + atan(calc_y / calc_x);
  calc_s = atan(calc_z / r) + cos((calc_z * calc_z + r * r + U * U - F * F) / (2 * U * sqrt(calc_z * calc_z + r * r)));
  calc_e = acos((U * U + F * F - calc_z * calc_z - r * r) / (2 * U * F));
}
