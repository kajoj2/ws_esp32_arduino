#ifndef __TMP112__
#define __TMP112__
#include  <stdint.h>
#include "Wire.h"


class TMP112 {
  public:
    TMP112();
    bool init_conitonus();
    float get_temperature_c();
};
#endif