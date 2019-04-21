#include <Arduino.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 2000

#define DIR1 2
#define STP1 3
#define DIR2 4
#define STP2 5
#define ENABLE 13 // optional (just delete ENABLE from everywhere if not used)

#include "DRV8825.h"
#define MODE0 10
#define MODE1 11
#define MODE2 12
DRV8825 stepper1(MOTOR_STEPS, DIR1, STP1, ENABLE, MODE0, MODE1, MODE2);
DRV8825 stepper2(MOTOR_STEPS, DIR2, STP2, ENABLE, MODE0, MODE1, MODE2);

void setup() {
    stepper1.begin(RPM);
    stepper1.enable();
    stepper2.begin(RPM);
    stepper2.enable();
}

void loop() {
    delay(1000);
    stepper1.setMicrostep(32);  // Set microstep mode to 1:1
    stepper2.setMicrostep(32);  // Set microstep mode to 1:1

    // One complete revolution is 360°
    stepper1.rotate(51*180);     // forward revolution
    stepper2.rotate(51*180);     // forward revolution
}
