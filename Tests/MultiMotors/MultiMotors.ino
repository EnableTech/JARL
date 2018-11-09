// From Microstepping demo and Echo demo
#include <SPI.h>
#include "DRV8825.h"

#define MOTOR_STEPS 200 // Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define RPM 120
#define MICROSTEPS 32

#define DIR_1 3
#define STEP_1 4
#define DIR_2 5
#define STEP_2 6
#define DIR_3 7
#define STEP_3 8

DRV8825 stepper1(MOTOR_STEPS, DIR_1, STEP_1);
DRV8825 stepper2(MOTOR_STEPS, DIR_2, STEP_2);
DRV8825 stepper3(MOTOR_STEPS, DIR_3, STEP_3);

int dirs[] = {DIR_1, DIR_2, DIR_3};
int steps[] = {STEP_1, STEP_2, STEP_3};
int vals[] = {100, 100, 100};
DRV8825 motors[] = {stepper1, stepper2, stepper3};

boolean enabled = true;
char c;

void setup() {
  Serial.begin(9600);
      delayMicroseconds(10);
  Serial.println("started");
  for (int i = 0; i < sizeof(motors); i++) {
    motors[i].begin(RPM);
    motors[i].setMicrostep(MICROSTEPS);
    pinMode(dirs[i], OUTPUT);
    pinMode(steps[i], OUTPUT);
    
  }
}

void loop() {
        
        //motors[0].move(MOTOR_STEPS);
        
  for (int j = 0; j < 1000; j++) {
    for (int i = 0; i < sizeof(motors); i ++) {
      digitalWrite(steps[i], HIGH);
      digitalWrite(dirs[i], HIGH);
      motors[i].rotate(-360); 
      Serial.println(i);
      delayMicroseconds(10);
    }
  }
  /*
  for (int i = 0; i < sizeof(dirs); i++) {
    digitalWrite(steps[i], LOW);
  }

  delayMicroseconds(1000);
  */
}
