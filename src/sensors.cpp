#include "sensors.h"

BH1730 lightSensor = BH1730();
TMP112 temperatureSensor = TMP112();
Dps310 Dps310PressureSensor = Dps310();
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
    // Call begin to initialize Dps310PressureSensor
    // The parameter 0x76 is the bus address. The default address is 0x77 and does not need to be given.
    // Dps310PressureSensor.begin(Wire, 0x76);
    // Use the commented line below instead to use the default I2C address.
    Dps310PressureSensor.begin(Wire, 0x76);

    // temperature measure rate (value from 0 to 7)
    // 2^temp_mr temperature measurement results per second
    int16_t temp_mr = 2;
    // temperature oversampling rate (value from 0 to 7)
    // 2^temp_osr internal temperature measurements per result
    // A higher value increases precision
    int16_t temp_osr = 2;
    // pressure measure rate (value from 0 to 7)
    // 2^prs_mr pressure measurement results per second
    int16_t prs_mr = 2;
    // pressure oversampling rate (value from 0 to 7)
    // 2^prs_osr internal pressure measurements per result
    // A higher value increases precision
    int16_t prs_osr = 2;
    // startMeasureBothCont enables background mode
    // temperature and pressure ar measured automatically
    // High precision and hgh measure rates at the same time are not available.
    // Consult Datasheet (or trial and error) for more information
    int16_t ret = Dps310PressureSensor.startMeasureBothCont(temp_mr, temp_osr, prs_mr, prs_osr);
    // Use one of the commented lines below instead to measure only temperature or pressure
    // int16_t ret = Dps310PressureSensor.startMeasureTempCont(temp_mr, temp_osr);
    // int16_t ret = Dps310PressureSensor.startMeasurePressureCont(prs_mr, prs_osr);

    return ret;
}


uint8_t DSP310_get_results(float *tempBuffer, uint8_t &tempCount, float *prsBuffer, uint8_t &prsCount){
    return Dps310PressureSensor.getContResults(tempBuffer, tempCount, prsBuffer, prsCount);
}

uint8_t SHTC3_init(){
    return mySHTC3.begin();
    //todo: setup high pressiosion mode
}

float SHTC3_measure_temperature_c(){
    mySHTC3.update();   
    return mySHTC3.toDegC();

}
float SHTC3_measure_huminidity(){
    mySHTC3.update();   
    return mySHTC3.toPercent();

}

