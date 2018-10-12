#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step. 1=full step, 2=half step etc.
#define MOTOR_STEPS 200
#define RPM 360
#define MICROSTEPS 4

// All the wires needed for full functionality
#define VERTICAL_DIR 2
#define VERTICAL_STEP 3
#define SHOULDER_DIR 4
#define SHOULDER_STEP 5
#define ELBOW_DIR 6
#define ELBOW_STEP 7
#define UPPER_ARM 14 //inches
#define FOREARM 14 //inches
#define MOVE_INC 0.5 //inches
#define VERT_DEG 30
#define HORZ_DEG 1

#define VERT_UP 8
#define VERT_DOWN 9
#define SHOULDER_OUT 10
#define SHOULDER_IN 11
#define ELBOW_OUT 12
#define ELBOW_IN 13 

//Uncomment line to use enable/disable functionality
//#define ENABLE 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver vertical(MOTOR_STEPS, VERTICAL_DIR, VERTICAL_STEP);
BasicStepperDriver shoulder(MOTOR_STEPS, SHOULDER_DIR, SHOULDER_STEP);
BasicStepperDriver elbow(MOTOR_STEPS, ELBOW_DIR, ELBOW_STEP);

int vu = 0;
int vd = 0;
int so = 0;
int si = 0;
int eo = 0;
int ei = 0;

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, ENABLE);

void setup() {    
    vertical.begin(RPM, MICROSTEPS);
    shoulder.begin(RPM, MICROSTEPS);
    elbow.begin(RPM, MICROSTEPS);
    pinMode(VERT_UP, INPUT);
    pinMode(VERT_DOWN, INPUT);
    pinMode(SHOULDER_OUT , INPUT);
    pinMode(SHOULDER_IN, INPUT);
    pinMode(ELBOW_OUT, INPUT);
    pinMode(ELBOW_IN, INPUT);    
    Serial.begin(9600);
}

void loop() {
    vu = digitalRead(VERT_UP);
    vd = digitalRead(VERT_DOWN);
    so = digitalRead(SHOULDER_OUT);
    si = digitalRead(SHOULDER_IN);
    eo = digitalRead(ELBOW_OUT);
    ei = digitalRead(ELBOW_IN);
    Serial.println(vu);
    Serial.println(vd);
    Serial.println(so);
    Serial.println(si);
    Serial.println(eo);
    Serial.println(ei);
    Serial.println();
    if(vu == HIGH && vd != HIGH) vertical.rotate(VERT_DEG);
    if(vd == HIGH && vu != HIGH) vertical.rotate(-VERT_DEG);
    if(so == HIGH && si != HIGH) shoulder.rotate(HORZ_DEG);
    if(si == HIGH && so != HIGH) shoulder.rotate(-HORZ_DEG);
    if(eo == HIGH && ei != HIGH) elbow.rotate(HORZ_DEG);
    if(ei == HIGH && eo != HIGH) elbow.rotate(-HORZ_DEG);
    delay(100);
}
