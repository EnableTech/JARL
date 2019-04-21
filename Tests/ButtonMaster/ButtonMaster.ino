#include <Wire.h>
#include <Arduino.h>
#include "DRV8825.h"

#define PAYLOAD_SIZE 1 // how many bytes to expect from each I2C slave node
#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 2000
#define MICROSTEPS 32
#define GEAR_RATIO 51

#define B_DIR 2
#define B_STP 3
#define S_DIR 4
#define S_STP 5
#define E_DIR 6
#define E_STP 7
#define ENABLE 13

#define MODE0 10
#define MODE1 11
#define MODE2 12

#define U 4
#define F 4
#define W 4

DRV8825 baseMotor(MOTOR_STEPS, B_DIR, B_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 shoulderMotor(MOTOR_STEPS, S_DIR, S_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 elbowMotor(MOTOR_STEPS, E_DIR, E_STP, ENABLE, MODE0, MODE1, MODE2);

int buttonVals;

void setup()
{
  Serial.begin(9600);
  Serial.println("MASTER");

  Wire.begin();        // Activate I2C link
  setupMotor(baseMotor);
  setupMotor(shoulderMotor);
  setupMotor(elbowMotor);
}

void loop()
{
  Wire.requestFrom(START_NODE, PAYLOAD_SIZE);    // request data from node#
  if (Wire.available() == PAYLOAD_SIZE) { // if data size is avaliable from nodes
    buttonVals = Wire.read();  // get nodes data
    Serial.println(buttonVals);
  }

  baseMotor.rotate(GEAR_RATIO * buttonVals);
  shoulderMotor.rotate(GEAR_RATIO * buttonVals);
  elbowMotor.rotate(GEAR_RATIO * buttonVals);

  delay(10);
}

void setupMotor(DRV8825 motor) {
  motor.begin(RPM);
  motor.enable();
  motor.setMicrostep(MICROSTEPS);
}
