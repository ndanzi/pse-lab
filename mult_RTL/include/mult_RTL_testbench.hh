#ifndef mult_RTL_TESTBENCH_H
#define mult_RTL_TESTBENCH_H

#define PERIOD 10

#include <systemc.h>

class mult_RTL_testbench : public sc_core::sc_module
{

private:

  void run();
  void clk_gen();

public:
  
  sc_in<sc_uint<1> >  	p_In_enable;
  sc_in<sc_lv<64> > 	  p_In_data;
  sc_out<sc_uint<1> >  	p_Out_enable;
  sc_out<sc_lv<64> > 	  p_Out_data1;
  sc_out<sc_lv<64> > 	  p_Out_data2;
  sc_out< bool > 		    reset_to_RTL;
  sc_out< sc_logic >    clk;

  SC_HAS_PROCESS(mult_RTL_testbench);
  mult_RTL_testbench(sc_module_name name);

};

#endif
