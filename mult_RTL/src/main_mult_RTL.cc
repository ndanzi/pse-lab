
#include "mult_RTL_testbench.hh"
#include "mult_RTL.hh"

int sc_main(int argc, char **argv)
{
  sc_signal< sc_logic >         clock;
  
  sc_signal < bool >        reset_signal;
  sc_signal<sc_lv<64> >     p_In_data1;
  sc_signal<sc_lv<64> >     p_In_data2;
  sc_signal<sc_uint<1> >    p_In_enable;
  sc_signal<sc_uint<1> >    p_Out_enable;
  sc_signal<sc_lv<64> >     p_Out_data;
  
  mult_RTL_testbench        i_src_RTL("src_RTL");       // source module
  mult_RTL                  i_mult_RTL("mult_RTL");    // mult RTL module

  i_mult_RTL.clk(clock);
  i_mult_RTL.reset(reset_signal);
  i_mult_RTL.numbers_areready(p_In_enable);
  i_mult_RTL.number1_port(p_In_data1);
  i_mult_RTL.number2_port(p_In_data2);
  i_mult_RTL.result_port(p_Out_data);
  i_mult_RTL.result_isready(p_Out_enable);
  
  i_src_RTL.clk(clock);
  i_src_RTL.p_In_enable(p_Out_enable);
  i_src_RTL.p_In_data(p_Out_data);
  i_src_RTL.p_Out_enable(p_In_enable);
  i_src_RTL.p_Out_data1(p_In_data1);
  i_src_RTL.p_Out_data2(p_In_data2);
  i_src_RTL.reset_to_RTL( reset_signal );
  
  sc_start();
  
  return 0;
};

