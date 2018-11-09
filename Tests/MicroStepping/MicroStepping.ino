#include <Arduino.h>
#include "DRV8825.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120
#define MICROSTEPS 32
#define UPPERARM 12
#define FOREARM 12

#define DIR1 3
#define STP1 4
#define DIR2 5
#define STP2 6
#define DIR3 7
#define STP3 8

DRV8825 stepper1(MOTOR_STEPS, DIR1, STP1);
DRV8825 stepper2(MOTOR_STEPS, DIR2, STP2);
DRV8825 stepper3(MOTOR_STEPS, DIR3, STP3);

//int dirs[] = {DIR1, DIR2, DIR3};
//int stps[] = {STP1, STP2, STP3};
//DRV8825 motors[] = {stepper1, stepper2, stepper3};
int vals[] = {10, 50, 100};
int thetas[] = {0, 0};

void setup() {
    stepper1.begin(RPM);
    stepper1.enable();    
    stepper1.setMicrostep(MICROSTEPS);
    stepper2.begin(RPM);
    stepper2.enable();    
    stepper2.setMicrostep(MICROSTEPS);
    stepper3.begin(RPM);
    stepper3.enable();
    stepper3.setMicrostep(MICROSTEPS);
}

void loop() {
    //getAngles();
    
    stepper1.move(10);
    stepper2.move(10);
    stepper3.move(10);
    delayMicroseconds(1);
}
