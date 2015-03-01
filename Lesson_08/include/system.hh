#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "testbench_TLM.hh"
#include "tlm_transactor.hh"
#include "tlm_transactor2.hh"
#include "rtl_transactor.hh"
#include "controller_error.hh"
#include "controller.hh"
#include "physical_plant.hh"
#include "mult_RTL.hh"

class System : public sc_core::sc_module
{
  public:
    System( sc_core::sc_module_name );
    ~System();

	
	testbench_TLM testbench;
	tlm_2_rtl tlm2rtl;
	tlm_2_ams tlm2ams;
	rtl_2_tdf rtl2tdf;
	error er;
	controller ctrl;
	plant plt;
  mult_RTL mult;
	
  sc_signal< sc_dt::sc_logic > clock;
  sc_signal < bool >        reset_signal;
  sc_signal<sc_lv<64> >     number1;
  sc_signal<sc_lv<64> >     number2;
  sc_signal<sc_uint<1> >    numbers_are_ready;
  sc_signal<sc_uint<1> >    result_is_ready;
  sc_signal<sc_lv<64> >     result;
	sc_core::sc_signal < sc_lv<64> >  r_rtl;
	sc_core::sc_signal < double >  y_rtl;
	
	
	sca_tdf::sca_signal<double> r;
	sca_tdf::sca_signal<double> e;
	sca_tdf::sca_signal<double> k;
	sca_tdf::sca_signal<double> y;

  SC_HAS_PROCESS(System);


	
	
	
};

#endif //SYSTEM_HH
