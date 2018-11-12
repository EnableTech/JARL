#include <ams_as5048b.h>
#define U_RAW 1
#define U_DEG 3

AMS_AS5048B mysensor(0x42);
AMS_AS5048B mysensor2(0x44);

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  mysensor.begin();
  mysensor2.begin();
}

void loop() {
  mysensor.angleR(U_RAW, true);
  Serial.print(mysensor.angleR(U_DEG, false)-90);
  Serial.print("  ||  ");
  mysensor2.angleR(U_RAW, true);
  Serial.print(mysensor2.angleR(U_DEG, false));
  Serial.print("\n");
  delay(1000);
}
