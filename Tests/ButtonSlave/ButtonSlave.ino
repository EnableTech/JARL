#include <Wire.h>

#define NODE_ADDRESS 1 // Change this unique address for each I2C slave node
#define PAYLOAD_SIZE 1 // Number of bytes  expected to be received by the master I2C node

#define DOWN 2
#define UP 3
#define BACK 4
#define FORWARD 5
#define LEFT 6
#define RIGHT 7
#define CLOSE 8
#define OPEN 9

byte nodePayload[PAYLOAD_SIZE];

#define PINS_LEN 8
int pins[] = {DOWN, UP, BACK, FORWARD, LEFT, RIGHT, CLOSE, OPEN};
int val = 0;

void setup() {
  Wire.begin(NODE_ADDRESS);  // Activate I2C network
  Wire.onRequest(requestEvent); // Request attention of master node
  for (int i = 0; i < PINS_LEN; i++) pinMode(pins[i], INPUT);
}

void loop() {}

void requestEvent() {
  val = 0;
  for (int i = 0; i < PINS_LEN; i++) {
    val = val << 1;
    val = val + digitalRead(pins[i]);
  }
  nodePayload[0] = val;
  Wire.write(nodePayload, PAYLOAD_SIZE);
}
