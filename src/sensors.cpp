#include "sensors.h"

BH1730 lightSensor = BH1730();
TMP112 temperatureSensor = TMP112();
Adafruit_DPS310 dps;
SHTC3 mySHTC3;

uint8_t BH1730_init()
{
    uint8_t ret_value = 0;
    ret_value += lightSensor.begin();
    lightSensor.setGain(GAIN_X128);
    return ret_value;
}

uint8_t BH1730_init_continous()
{
    return -1;
}

float BH1730_oneTimeMessurments()
{
    return lightSensor.readLux();
}

uint8_t TMP112_init_conitonus()
{
    return temperatureSensor.init_conitonus();
}
float TMP112_get_temperature_c()
{
    return temperatureSensor.get_temperature_c();
}

uint8_t DSP310_init_continous()
{
    dps.begin_I2C(0x76);
    dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
    dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    return 0;
}

// uint8_t DSP310_get_results(float *tempBuffer, uint8_t &tempCount, float *prsBuffer, uint8_t &prsCount)
// {
//     return Dps310PressureSensor.getContResults(tempBuffer, tempCount, prsBuffer, prsCount);
// }

float DSP310_measure_pressure(){
     sensors_event_t temp_event, pressure_event;
  
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    delay(10);
  }

  dps.getEvents(&temp_event, &pressure_event);
 // Serial.print(temp_event.temperature);
  //Serial.print(pressure_event.pressure);

return pressure_event.pressure;
}

uint8_t SHTC3_init()
{
    return mySHTC3.begin();
    // todo: setup high pressiosion mode
}

float SHTC3_measure_temperature_c()
{
    mySHTC3.update();
    return mySHTC3.toDegC();
}
float SHTC3_measure_huminidity()
{
    mySHTC3.update();
    return mySHTC3.toPercent();
}
