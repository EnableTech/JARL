/**************************************************************************/
/*!
    @file     read_simple_angle.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

	read a simple angle from AS5048B over I2C bus

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/

#include <ams_as5048b.h>

//unit consts
#define U_RAW 1
//#define U_TRN 2
#define U_DEG 3
//#define U_RAD 4
//#define U_GRAD 5
//#define U_MOA 6
//#define U_SOA 7
//#define U_MILNATO 8
//#define U_MILSE 9
//#define U_MILRU 10

//AMS_AS5048B mysensor((uint8_t) 64);
AMS_AS5048B mysensor(0x42);
AMS_AS5048B mysensor2(0x44);

void setup() {

	//Start serial
	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready

	//Start Wire object. Unneeded here as this is done (optionally) by the AMS_AS5048B object (see lib code - #define USE_WIREBEGIN_ENABLED)
	//Wire.begin();

	//init AMS_AS5048B object
	mysensor.begin();

  //init AMS_AS5048B object 2
  mysensor2.begin();

	//consider the current position as zero
//	mysensor.setZeroReg();
//  mysensor2.setZeroReg();




  //Testing I2C Addresses?
//  byte rc;
//  byte start_address = 0;
//  byte end_address = 127;
//  Serial.print("Scanning I2C bus from ");
//  Serial.print(start_address,DEC);  Serial.print(" to ");  Serial.print(end_address,DEC);
//  Serial.println("...");
//
//  for( byte addr  = start_address;
//            addr <= end_address;
//            addr++ ) {
//      Wire.beginTransmission(addr);
//      rc = Wire.endTransmission();
//
//      if (addr<16) Serial.print("0");
//      Serial.print(addr,HEX);
//      if (rc==0) {
//        Serial.print(" found!");
//      } else {
//        Serial.print(" "); Serial.print(rc); Serial.print("     ");
//      }
//      Serial.print( (addr%8)==7 ? "\n":" ");
//  }

}

void loop() {
  //print to seriod the raw angle and degree angle every 1 second
  //print 2 times the same angle: Raw, Deg, Address || Raw, Deg, Address
  Serial.print(mysensor.angleR(U_RAW,true));
  Serial.print(", ");
  Serial.print(mysensor.angleR(U_DEG, false));
//  Serial.print(", ");
//  Serial.print(mysensor.addressRegR());
  Serial.print("  ||  ");
  Serial.print(mysensor2.angleR(U_RAW, true));
  Serial.print(", ");
  Serial.print(mysensor2.angleR(U_DEG, false));
//  Serial.print(", ");
//  Serial.print(mysensor2.addressRegR());
  Serial.print("\n");

	delay(10);
}
