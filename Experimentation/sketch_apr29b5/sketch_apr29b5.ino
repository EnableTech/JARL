int dir1 = 3;
int step1 = 4;
int dir2 = 5;
int step2 = 6;
int steps = 360;
void setup() {
  pinMode(dir1, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step2, OUTPUT);
}
void step(boolean d1, boolean d2, int speed1, int speed2) {
  digitalWrite(dir1, d1);
  digitalWrite(dir2, d2);
  for (int i = 0; i < steps; i++) {
    if (i % 2 == 0 && i % 4 == 0) {
      digitalWrite(step1, HIGH);
      digitalWrite(step2, HIGH);
    }
    else{
      digitalWrite(step2, HIGH);
    }
    delayMicroseconds(10);
    digitalWrite(step1, LOW);
    digitalWrite(step2, LOW);
    delayMicroseconds(1);

  }
}
void loop() {
  step(true, false, 10, 100);
  delay(50);
}
