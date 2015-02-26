#ifndef __define_TLM_h__
#define __define_TLM_h__

#include <systemc>

struct iostruct {
  double y;
  double r;
  sc_lv<64> number1;
  sc_lv<64> number2;
  sc_lv<64> result;
};

#define ADDRESS_TYPE int
#define DATA_TYPE iostruct

#endif
