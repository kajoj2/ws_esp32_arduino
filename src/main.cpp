#include <Arduino.h>
#define I2C_SDA 21
#define I2C_SCL 22
#include "sensors.h"

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.setPins(I2C_SDA, I2C_SCL);
  Wire.begin(I2C_SDA, I2C_SCL);
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  BH1730_init();
  TMP112_init_conitonus();
  DSP310_init_continous();
  SHTC3_init();

  delay(300);
}

void loop()
{
  // Output data to serial monitor
  Serial.print("Temperature in Celsius  TMP112 ");
  Serial.print(TMP112_get_temperature_c());
  Serial.println(" C");

  Serial.print("");
  Serial.print(BH1730_oneTimeMessurments());
  Serial.println(" lux");


uint8_t pressureCount = 20;
  float pressure[pressureCount];
  uint8_t temperatureCount = 20;
 float temperature[temperatureCount];

  //This function writes the results of continuous measurements to the arrays given as parameters
  //The parameters temperatureCount and pressureCount should hold the sizes of the arrays temperature and pressure when the function is called
  //After the end of the function, temperatureCount and pressureCount hold the numbers of values written to the arrays
  //Note: The Dps310 cannot save more than 32 results. When its result buffer is full, it won't save any new measurement results
  int16_t ret = DSP310_get_results(temperature, temperatureCount, pressure, pressureCount);

  if (ret != 0)
  {
    Serial.println();
    Serial.println();
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.println();
    Serial.println();
    Serial.print(temperatureCount);
    Serial.println(" temperature values found: ");
    for (int16_t i = 0; i < temperatureCount; i++)
    {
      Serial.print(temperature[i]);
      Serial.println(" degrees of Celsius");
    }

    Serial.println();
    Serial.print(pressureCount);
    Serial.println(" pressure values found: ");
    for (int16_t i = 0; i < pressureCount; i++)
    {
      Serial.print(pressure[i]);
      Serial.println(" Pascal");
    }
  }


  Serial.print("Temperature in Celsius:  SHTC3  ");
  Serial.print(SHTC3_measure_temperature_c());
  Serial.println(" C");


  Serial.print("huminidity SHTC3  ");
  Serial.print(SHTC3_measure_huminidity());
  Serial.println(" %");

  delay(500);
}