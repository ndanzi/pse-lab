#ifndef mult_RTL_H
#define mult_RTL_H

#include "systemc.h"



SC_MODULE(mult_RTL){

  sc_in<sc_uint<1> >    numbers_areready;
  sc_in<sc_lv<64> >     number1_port;
  sc_in<sc_lv<64> >     number2_port;
  sc_out<sc_lv<64> >    result_port;
  //sc_out<sc_lv<64> >    r_port;
  sc_out<sc_uint<1> >   result_isready;
  sc_in<bool>           reset;
  sc_in< sc_dt::sc_logic > clk;

  typedef enum {Reset_ST, ST_0, ST_1, ST_2, ST_3, ST_4, ST_5, ST_6, ST_7, ST_8, ST_9, ST_10, ST_11, ST_12} STATES;

  sc_signal<STATES> STATUS, NEXT_STATUS;
  sc_signal<bool >	  overflow;
  sc_signal<bool >   stop_m_mult;
  sc_signal<bool >   normalized;
  

  void elaborate_MULT(void);
  void elaborate_MULT_FSM(void);

  SC_CTOR(mult_RTL){
    SC_METHOD(elaborate_MULT_FSM);
    sensitive << reset.neg();
    sensitive << clk.pos();

     

    SC_METHOD(elaborate_MULT);
    sensitive << STATUS << numbers_areready << number1_port << number2_port;
  };
};

#endif
