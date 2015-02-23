#ifndef __define_TLM_h__
#define __define_TLM_h__

#include <systemc>

struct iostruct {
  double r;
  double y;
};

#define ADDRESS_TYPE int
#define DATA_TYPE iostruct

#endif
