#ifndef __define_LT_h__
#define __define_LT_h__

#include <systemc.h>

struct iostruct {
  sc_lv<64> number1;
  sc_lv<64> number2;
  sc_lv<64> result;
};

#define ADDRESS_TYPE int
#define DATA_TYPE iostruct

#endif
