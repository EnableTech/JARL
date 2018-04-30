#define pinNum 6
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinNum, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(pinNum));
  delay(100);
}
