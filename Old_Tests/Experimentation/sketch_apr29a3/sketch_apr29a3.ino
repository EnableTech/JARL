int dir1 = 3;
int step1 = 4;
int dir2 = 5;
int step2 = 6;
void setup() {
  pinMode(dir1, OUTPUT);
  pinMode(step1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step2, OUTPUT);
}
void step(boolean d1, boolean d2, int steps, int speed1, int speed2) {
  digitalWrite(dir1, d1);
  digitalWrite(dir2, d2);
  delay(50);
  for (int i = 0; i < steps; i++) {
    digitalWrite(step1, HIGH);
    digitalWrite(step2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(step1, LOW);
    delayMicroseconds(1000);
    digitalWrite(step2, LOW);
    delayMicroseconds(100);

  }
}
void loop() {
  step(true, false, 1600, 900, 100);
  delay(50);
  step(true, false, 1600, 900, 100);
  delay(50);
}
