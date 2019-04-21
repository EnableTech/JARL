#include <Servo.h>

#define P1 2
#define P2 3


Servo servo1;
Servo servo2;

int pos = 0;

void setup() {
  servo1.attach(P1);
  servo2.attach(P2);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    moveBothToPos();
    delay(10);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    moveBothToPos();
    delay(10);
  }
}

void moveBothToPos() {
  servo1.write(pos);
  servo2.write(-pos);
}

