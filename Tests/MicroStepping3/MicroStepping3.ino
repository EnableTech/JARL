#include <Arduino.h>

#define MOTOR_STEPS 200
#define RPM 2000

#define DIR1 2
#define STEP1 3
#define DIR2 4
#define STEP2 5
#define ENABLE 13 // optional (just delete ENABLE from everywhere if not used)

#include "DRV8825.h"
#define MODE0 10
#define MODE1 11
#define MODE2 12
DRV8825 stepper1(MOTOR_STEPS, DIR1, STEP1, ENABLE, MODE0, MODE1, MODE2);
DRV8825 stepper2(MOTOR_STEPS, DIR2, STEP2, ENABLE, MODE0, MODE1, MODE2);

void setup() {
    stepper1.begin(RPM);
    stepper1.enable();
    stepper2.begin(RPM);
    stepper2.enable();
}

void loop() {
    stepper1.setMicrostep(32);
    stepper1.rotate(51*360);  
    stepper2.setMicrostep(32);
    stepper2.rotate(51*360);  
    delay(1000);
}
