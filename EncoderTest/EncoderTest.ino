#include <Arduino.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

#define S_STEP 8
#define S_DIR 9
//#define E_STEP 100
//#define E_DIR 100
#define ENABLE 4 

#include "DRV8825.h"
#define MODE0 5
#define MODE1 6
#define MODE2 7
DRV8825 shoulder(MOTOR_STEPS, S_DIR, S_STEP, ENABLE, MODE0, MODE1, MODE2);
//DRV8825 elbow(MOTOR_STEPS, E_DIR, E_STEP, ENABLE, MODE0, MODE1, MODE2);

#define RIGHT 2
#define LEFT 3
//#define FORWARD 100
//#define BACK 100

int r = 0;
int l = 0;
//int f = 0;
//int b = 0;
int microstep = 32;
int stp = 1;

int sAngle = 0;
int eAngle = 0;
int currSAngle = 0;
int currEAngle = 0;

void setup() {
    
    shoulder.begin(RPM);
    //elbow.begin(RPM);
    shoulder.enable();

    pinMode(RIGHT, INPUT);
    pinMode(LEFT, INPUT);
    //pinMode(FORWARD, INPUT);
    //pinMode(BACK, INPUT);

    shoulder.setMicrostep(microstep);   // Set microstep mode    
    //elbow.setMicrostep(microstep);   // Set microstep mode

    Serial.begin(9600);
}

void loop() {
    delay(5);

    //currSAngle = encoderPos();
    //currEAngle = encoderPos();

    r = digitalRead(RIGHT);
    l = digitalRead(LEFT);
    //f = digitalRead(FORWARD);
    //b = digitalRead(BACK);

    Serial.print(l);
    Serial.print(r);
    //Serial.print(f);
    //Serial.print(b);
    Serial.println(); 
    
    if(r != l){
        if(r == HIGH){
            shoulder.move(microstep * stp);    // forward revolution
        }
        else{
            shoulder.move(-microstep * stp);   // reverse revolution
        }
    }    
    // One complete revolution is still 360° regardless of microstepping mode
    // rotate() is easier to use than move() when no need to land on precise microstep position
    //stepper.rotate(360);
    //stepper.rotate(-360);
}

int encoderPos(){return 0;}

