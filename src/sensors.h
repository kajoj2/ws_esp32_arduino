#ifndef __SENSORS__
#define __SENSORS__

#define DPS_DISABLESPI
#include <stdint.h>
#include <BH1730.h>
#include <TMP112.h>
//#include <Dps310.h>
#include <Adafruit_DPS310.h>
#include "SparkFun_SHTC3.h"

uint8_t BH1730_init();
uint8_t BH1730_init_continous();
float BH1730_oneTimeMessurments();

uint8_t TMP112_init_conitonus();
float TMP112_get_temperature_c();

uint8_t DSP310_init_continous();
// uint8_t DSP310_get_results(float *tempBuffer, uint8_t &tempCount, float *prsBuffer, uint8_t &prsCount);
float DSP310_measure_pressure();

uint8_t SHTC3_init();
float SHTC3_measure_temperature_c();
float SHTC3_measure_huminidity();


#endif