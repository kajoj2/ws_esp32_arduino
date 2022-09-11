#include "TMP112.h"

#include "Wire.h"

TMP112::TMP112() {}

#define Addr 0x48

bool  TMP112::init_conitonus(){
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select configuration register
  Wire.write(0x01);
  // Continuous conversion, comparator mode, 12-bit resolution
  Wire.write(0x60);
  Wire.write(0xA0);
  // Stop I2C Transmission
  Wire.endTransmission();
  return 0;
}
float  TMP112::get_temperature_c(){
    unsigned data[2];
   
   // Start I2C Transmission
   Wire.beginTransmission(Addr);
   // Select data register
   Wire.write(0x00);
   // Stop I2C Transmission
   Wire.endTransmission();
   delay(300);

   // Request 2 bytes of data
   Wire.requestFrom(Addr, 2);


   // Read 2 bytes of data
   // temp msb, temp lsb
   if(Wire.available() == 2)
   {
     data[0] = Wire.read();
     data[1] = Wire.read();
   }
   // Convert the data to 12-bits
   int temp = ((data[0] * 256) + data[1]) / 16;
   if(temp > 2048)
   {
     temp -= 4096;
   }

   float cTemp = temp * 0.0625;
   return cTemp;
}