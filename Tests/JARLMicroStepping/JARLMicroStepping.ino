#include <Arduino.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 2000

#define DIR 8
#define STEP 9
#define ENABLE 13 // optional (just delete ENABLE from everywhere if not used)

#include "DRV8825.h"
#define MODE0 10
#define MODE1 11
#define MODE2 12
DRV8825 stepper(MOTOR_STEPS, DIR, STEP, ENABLE, MODE0, MODE1, MODE2);

void setup() {
    stepper.begin(RPM);
    stepper.enable();
}

void loop() {
    delay(1000);
    stepper.setMicrostep(32);  // Set microstep mode to 1:1

    // One complete revolution is 360°
    stepper.rotate(51*360);     // forward revolution
}
