#include <Wire.h>
#include <Servo.h>

// I2C specs
#define NODE_ADDRESS 1 // Change this unique address for each I2C slave node
#define PAYLOAD_SIZE 1 // Number of bytes  expected to be received by the master I2C node

// Servo pins and limits
#define S1 4
#define S2 5
#define UL 180
#define LL 0

// Button pins
#define PINS_LEN 8
#define RGHT 6
#define LEFT 7
#define FWRD 8
#define BACK 9
#define UPWD 10
#define DOWN 11
#define OPEN 12
#define CLOS 13

// I2C payload
byte payload[PAYLOAD_SIZE];

// Servos
Servo servo1;
Servo servo2;

// For reading values
int pins[] = {RGHT, LEFT, FWRD, BACK, UPWD, DOWN, OPEN, CLOS};
int pos = 90;
int temp_pos = 0;
int incr = 10;
bool r = false;
bool l = false;
bool f = false;
bool b = false;
bool u = false;
bool d = false;
bool o = false;
bool c = false;

void setup() {
  Wire.begin(NODE_ADDRESS);  // Activate I2C network
  Wire.onRequest(requestEvent); // Request attention of master node
  for (int i = 0; i < PINS_LEN; i++) pinMode(pins[i], INPUT); // Button setup
  // Servo setup
  servo1.attach(S1);
  servo2.attach(S2);
}

void loop() {
  readPins();
  temp_pos = pos + incr * (o - c);
  if (temp_pos >= LL && temp_pos <= UL) {
    pos = temp_pos;
    moveBothToPos();
  }
}

void requestEvent() {
  readPins();
  payload[0] = pow(2, 7) * r + pow(2, 6) * l + pow(2, 5) * f + pow(2, 4) * b + pow(2, 3) * u + pow(2, 2) * d;
  Wire.write(payload, PAYLOAD_SIZE);
}

void readPins() {
  r = digitalRead(RGHT);
  l = digitalRead(LEFT);
  f = digitalRead(FWRD);
  b = digitalRead(BACK);
  u = digitalRead(UPWD);
  d = digitalRead(DOWN);
  o = digitalRead(OPEN);
  c = digitalRead(CLOS);
}

void moveBothToPos() {
  servo1.write(pos);
  servo2.write(-pos);
}
