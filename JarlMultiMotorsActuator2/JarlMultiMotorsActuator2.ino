// From Microstepping demo and Echo demo
#include <SPI.h>
#include <Servo.h>
#include "Adafruit_BLE_UART.h"
#include "DRV8825.h"

#define MOTOR_STEPS 200 // Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define RPM 120
#define MICROSTEPS 32

#define LINEAR50PIN 10

#define LINEAR50_MIN  1050     //
#define LINEAR50_MAX  2000    //

#define DIR_1 3
#define STEP_1 4
#define DIR_2 5
#define STEP_2 6
#define DIR_3 7
#define STEP_3 8

DRV8825 stepper1(MOTOR_STEPS, DIR_1, STEP_1);
DRV8825 stepper2(MOTOR_STEPS, DIR_2, STEP_2);
DRV8825 stepper3(MOTOR_STEPS, DIR_3, STEP_3);

Servo LINEAR50; 
int linear50Value = 0;
boolean direction = true;

int dirs[] = {DIR_1, DIR_2, DIR_3};
int steps[] = {STEP_1, STEP_2, STEP_3};
DRV8825 motors[] = {stepper1, stepper2, stepper3};

// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

char codes[] = {'f', 'b', 'l', 'r', 'u', 'd'};
boolean enabled = true;
boolean settings[] = {false, false, false, false, false, false};
char c;

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void setup(void) {
  Serial.begin(9600);
  while (!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Bluetooth Started"));

  //BTLEserial.setDeviceName("JARL"); // 7 characters max

  BTLEserial.begin();
  for(int i = 0; i < sizeof(motors); i++){
      motors[i].begin(RPM);
      motors[i].setMicrostep(MICROSTEPS);  
      pinMode(dirs[i], OUTPUT);
      pinMode(steps[i], OUTPUT);
  }
  LINEAR50.attach(LINEAR50PIN, LINEAR50_MIN, LINEAR50_MAX);
  LINEAR50.writeMicroseconds(linear50Value);
}

void loop() {
  // Tell the nRF8001 to do whatever it should be working on.
  BTLEserial.pollACI();

  // Ask what is our current status
  aci_evt_opcode_t status = BTLEserial.getState();
  // If the status changed....
  if (status != laststatus) {
    // print it out!
    if (status == ACI_EVT_DEVICE_STARTED) {
      Serial.println(F("* Advertising started"));
    }
    if (status == ACI_EVT_CONNECTED) {
      Serial.println(F("* Connected!"));
    }
    if (status == ACI_EVT_DISCONNECTED) {
      Serial.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    laststatus = status;
  }

  if (status == ACI_EVT_CONNECTED) {
    if (BTLEserial.available() == 2) {
      c = BTLEserial.read();
      Serial.print(c);
      if (c == 's') {
        enabled = false;
      }
      for (int i = 0; i < sizeof(codes); i++) {
        if (c == codes[i]) {
          c = BTLEserial.read();
          Serial.print(c);
          settings[i] = (boolean)(c - 48);
          Serial.println(settings[i]);
        }
      }
    }

    if (enabled) {
      for (int i = 0; i < sizeof(settings); i += 2) {
        if (settings[i] && !settings[i + 1]) {
          digitalWrite(steps[i/2], HIGH);
          digitalWrite(dirs[i/2], HIGH);
          //motors[i / 2].move(MICROSTEPS / 2);
          Serial.println(codes[i]);
        }
        else if (!settings[i] && settings[i + 1]) {
          digitalWrite(steps[i/2], HIGH);
          digitalWrite(dirs[i/2], LOW);
          //motors[i / 2].move(-MICROSTEPS / 2);
          Serial.println(codes[i + 1]);
        }
      }
      delayMicroseconds(1000);
      for(int i = 0; i < sizeof(dirs); i++){
        digitalWrite(steps[i], LOW);
      }
    }
    linear50Value++;
    if(
    linear50Value = linear50Value%1023;
    int value = map(linear50Value, 0, 1023, LINEAR50_MAX, LINEAR50_MIN); //Map analog value from the sensor to the linear actuator
    LINEAR50.writeMicroseconds(value);
  }
}
