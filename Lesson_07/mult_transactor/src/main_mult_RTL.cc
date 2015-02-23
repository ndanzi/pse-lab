#include <systemc.h>
#include <tlm.h>
#include "mult_LT_testbench.hh"
#include "mult_RTL_transactor.hh"
#include "mult_RTL.hh"

#define PERIOD 10

SC_MODULE(main_RTL)
{
  
  // CLOCK SIGNAL
  sc_signal< sc_dt::sc_logic > clock;

  void clk_gen()
  {
    while( true )
    {
      clock.write( sc_dt::SC_LOGIC_1 );
      wait( PERIOD / 2, sc_core::SC_NS );
      clock.write( sc_dt::SC_LOGIC_0 );
      wait( PERIOD / 2, sc_core::SC_NS );   
    }
  }

  //Signals
  sc_signal < bool >        reset_signal;
  sc_signal<sc_lv<64> >     p_In_data1;
  sc_signal<sc_lv<64> >     p_In_data2;
  sc_signal<sc_uint<1> >    p_In_enable;
  sc_signal<sc_uint<1> >    p_Out_enable;
  sc_signal<sc_lv<64> >     p_Out_data;

  //Definition   
  mult_LT_testbench		i_src_LT;	       // source module
  mult_RTL_transactor		i_mult_RTL_transactor; // mult transactor  
  mult_RTL			i_mult_RTL;    // mult RTL module
    
  //Construct
  SC_CTOR(main_RTL)
    : i_src_LT("i_src_LT")
    , i_mult_RTL_transactor("i_mult_RTL_transactor")
    , i_mult_RTL("i_mult_RTL")
  {
      
    SC_THREAD( clk_gen );

    // TLM testbench <-> Transactor binding (TLM interface)
    i_src_LT.initiator_socket(i_mult_RTL_transactor.target_socket);
	
    // RTL design <-> Transactor binding (RTL interfaces)
    i_mult_RTL.clk(clock);
    i_mult_RTL.reset(reset_signal);
    i_mult_RTL.numbers_areready(p_In_enable);
    i_mult_RTL.number1_port(p_In_data1);
    i_mult_RTL.number2_port(p_In_data2);
    i_mult_RTL.result_port(p_Out_data);
    i_mult_RTL.result_isready(p_Out_enable);
   


    i_mult_RTL_transactor.clk(clock);
    i_mult_RTL_transactor.p_In_enable(p_Out_enable);
    i_mult_RTL_transactor.p_In_data(p_Out_data);
    i_mult_RTL_transactor.p_Out_enable(p_In_enable);
    i_mult_RTL_transactor.p_Out_data1(p_In_data1);
    i_mult_RTL_transactor.p_Out_data2(p_In_data2);
    i_mult_RTL_transactor.reset_to_RTL( reset_signal );
 
	
  }





};

int sc_main(int argc, char **argv)
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  main_RTL* topl;

  topl = new main_RTL("topl");

  sc_start();

  return(0);

};


