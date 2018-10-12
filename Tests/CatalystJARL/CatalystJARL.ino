// From Microstepping demo and Echo demo
#include <SPI.h>
#include "Adafruit_BLE_UART.h"
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
DRV8825 motors[] = {stepper1, stepper2, stepper3};

// Connect CLK/MISO/MOSI to hardware SPI
// e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
/*
String codes[] = {"f0", "f1",
                  "b0", "b1",
                  "l0", "l1",
                  "r0", "r1",
                  "u0", "u1",
                  "d0", "d1"
                 };
*/
String codes[][2] = {{"!B50", "!B51"},
                  {"!B60", "!B61"},
                  {"!B70", "!B71"},
                  {"!B80", "!B81"},
                  {"!B10", "!B11"},
                  {"!B30", "!B31"}
                 };
String messages[] = {"front", "back", "left", "right", "up", "down"};

boolean forward = false;
boolean back = false;
boolean left = false;
boolean right = false;
boolean up = false;
boolean down = false;
boolean enabled = true;
boolean settings[] = {forward, back, left, right, up, down};
String string;

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void setup(void) {
  Serial.begin(9600);
  
  while (!Serial); // Leonardo/Micro should wait for serial init
  
  Serial.println(F("Bluetooth Started"));

  BTLEserial.setDeviceName("JARL"); // 7 characters max

  BTLEserial.begin();

  stepper1.begin(RPM);
  stepper2.begin(RPM);
  stepper3.begin(RPM);
}

void loop() {
  if (enabled) {
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
      // Lets see if there's any data for us!
      if (BTLEserial.available()) {
        Serial.print("* "); Serial.print(BTLEserial.available()); Serial.println(F(" bytes available from BTLE"));
      }
      // OK while we still have something to read, get a character and print it out
      string = "";
      while (BTLEserial.available()) {
        char c = BTLEserial.read();
        string +=c;
      }
      //Serial.print(string);
      
      for (int i = 0; i < sizeof(codes); i++) {
        for(int j = 0; j < 2; j++){
        if (string.length() > 3 && string.charAt(0) == codes[i][j].charAt(0) && string.charAt(1) == codes[i][j].charAt(1) && string.charAt(2) == codes[i][j].charAt(2) && string.charAt(3) == codes[i][j].charAt(3)) {          
          settings[i] = (boolean)j;
        }
        }
      }

      for (int i = 0; i < sizeof(settings); i += 2) {
        if (settings[i] && !settings[i + 1]) {
          motors[i/2].move(MICROSTEPS/2);
          Serial.println(messages[i]);
        }
        else if (!settings[i] && settings[i + 1]) {
          motors[i/2].move(-MICROSTEPS/2);
          Serial.println(messages[i + 1]);
        }
      }      
    }
  }
}
