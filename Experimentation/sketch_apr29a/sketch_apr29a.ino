int dirPin = 3;
int stepperPin = 4;
int dir2 = 5;
int step2 = 6;
void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepperPin, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(step2, OUTPUT);
}
void step(boolean dir, int steps, int speed1, int speed2) {
  digitalWrite(dirPin, dir);
  digitalWrite(dir2, dir);
  delay(50);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    digitalWrite(step2, HIGH);
    delayMicroseconds(speed2);    
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(speed1 - speed2);
    digitalWrite(step2, LOW);
    delayMicroseconds(1000 - speed1);
  }
}
void loop() {
  step(true, 1600, 900, 0);
  delay(500);
  step(false, 1600, 900, 0);
  delay(500);
}
