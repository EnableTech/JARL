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
  for (int i = 0; i < speed1; i++) {
    digitalWrite(stepperPin, HIGH);
    if(i < speed2){
    digitalWrite(step2, HIGH);
    }
    delayMicroseconds(800);    
    digitalWrite(stepperPin, LOW);
    digitalWrite(step2, LOW);
    delayMicroseconds(800);
  }
}
void loop() {
  step(true, 1600, 900, 100);
  delay(500);
  step(false, 1600, 900, 100);
  delay(500);
}
