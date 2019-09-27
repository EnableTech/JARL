#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// I2C specs
#define NODE_ADDRESS 1 // Change this unique address for each I2C slave node
#define PAYLOAD_SIZE 1 // Number of bytes  expected to be received by the master I2C node

// Servo pins and limits
#define S1 4
#define S2 5
#define UL 90
#define LL 0

#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

// I2C payload
byte payload[PAYLOAD_SIZE];

// Servos
Servo servo1;
Servo servo2;

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

bool r = false;
bool l = false;
bool f = false;
bool b = false;
bool u = false;
bool d = false;
bool o = false;
bool c = false;
char codes[] = {'r', 'l', 'f', 'b', 'u', 'd', 'o', 'c'};
bool stopped = false;
bool settings[] = {false, false, false, false, false, false, false, false};
char val;

int pos = 90;
int temp_pos = 0;
int incr = 1;

String messages[] = {"rght", "left", "fwrd", "back", "upwd", "down", "open", "clos"};

aci_evt_opcode_t laststatus = ACI_EVT_DISCONNECTED;

void setup(void) {
  Serial.begin(9600);
  while (!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Bluetooth Started"));

  //BTLEserial.setDeviceName("JARL"); // 7 characters max

  BTLEserial.begin();

  Wire.begin(NODE_ADDRESS);  // Activate I2C network
  Wire.onRequest(requestEvent); // Request attention of master node
  //for (int i = 0; i < PINS_LEN; i++) pinMode(pins[i], INPUT); // Button setup
  // Servo setup
  servo1.attach(S1);
  servo2.attach(S2);
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
      val = BTLEserial.read();
      Serial.print(val);
      if (val == 's') {
        stopped = true;
      }
      for (int i = 0; i < sizeof(codes); i++) {
        if (val == codes[i]) {
          val = BTLEserial.read();
          Serial.print(val);
          settings[i] = (bool)(val - 48);
          Serial.println(settings[i]);
        }
      }
    }
    if (!stopped) {
      for (int i = 0; i < sizeof(settings); i++) {
        if (settings[i]) {
          Serial.println(messages[i]);
        }
      }
      readPins();
      temp_pos = pos + incr * (o - c);
      if (temp_pos >= LL && temp_pos <= UL) {
        pos = temp_pos;
        moveBothToPos();
      }
      delay(10);
    }
  }
}

void requestEvent() {
  readPins();
  payload[0] = pow(2, 7) * r + pow(2, 6) * l + pow(2, 5) * f + pow(2, 4) * b + pow(2, 3) * u + pow(2, 2) * d;
  Wire.write(payload, PAYLOAD_SIZE);
}

void readPins() {
  r = settings[0];
  l = settings[1];
  f = settings[2];
  b = settings[3];
  u = settings[4];
  d = settings[5];
  o = settings[6];
  c = settings[7];
}

void moveBothToPos() {
  servo1.write(180 + pos);
  servo2.write(180 - pos);
}
