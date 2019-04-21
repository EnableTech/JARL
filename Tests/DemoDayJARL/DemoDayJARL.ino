#include <Arduino.h>
#include "DRV8825.h"
#include <Wire.h>

#define PAYLOAD_SIZE 1 // how many bytes to expect from each I2C slave node
#define NODE_MAX 1 // maximum number of slave nodes (I2C addresses) to probe
#define START_NODE 1 // The starting I2C address of slave nodes

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 2000
#define MICROSTEPS 32

#define BASE_DIR 2
#define BASE_STP 3
#define SHLD_DIR 4
#define SHLD_STP 5
#define ELBW_DIR 6
#define ELBW_STP 7
#define WRST_DIR 8
#define WRST_STP 9

#define MODE0 10
#define MODE1 11
#define MODE2 12
#define ENABLE 13

DRV8825 base(MOTOR_STEPS, BASE_DIR, BASE_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 shld(MOTOR_STEPS, SHLD_DIR, SHLD_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 elbw(MOTOR_STEPS, ELBW_DIR, ELBW_STP, ENABLE, MODE0, MODE1, MODE2);
DRV8825 wrst(MOTOR_STEPS, WRST_DIR, WRST_STP, ENABLE, MODE0, MODE1, MODE2);

byte payload[PAYLOAD_SIZE];

void setup() {
  Wire.begin(NODE_ADDRESS);  // Activate I2C network
  Wire.onRequest(requestEvent); // Request attention of master node
  
  setupMotor(base);
  setupMotor(shld);
  setupMotor(elbw);
  setupMotor(wrst);
}

void loop() {
  delay(1000);
  
  // One complete revolution is 360°
  stepper1.rotate(51 * 180);   // forward revolution
  stepper2.rotate(51 * 180);   // forward revolution
}

void setupMotor(DRV8825 motor) {
  motor.begin(RPM);
  motor.enable();
  motor.setMicrostep(MICROSTEPS);
}

void requestEvent() {
  val = 0;
  for (int i = 0; i < PINS_LEN; i++) {
    val = val << 1;
    val = val + digitalRead(pins[i]);
  }
  payload[0] = val;
  Wire.write(payload, PAYLOAD_SIZE);
}
