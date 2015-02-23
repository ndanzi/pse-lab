#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "testbench_TLM.hh"
#include "tlm_transactor.hh"
#include "rtl_transactor.hh"
#include "controller_error.hh"
#include "controller.hh"
#include "physical_plant.hh"

class System : public sc_core::sc_module
{
  public:
    System( sc_core::sc_module_name );
    ~System();

	
	testbench_TLM testbench;
	tlm_2_rtl tlm2rtl;
	rtl_2_tdf rtl2tdf;
	error er;
	controller ctrl;
	plant plt;
	
	
	sc_core::sc_signal < double >  r_rtl;
	sc_core::sc_signal < double >  y_rtl;
	
	
	sca_tdf::sca_signal<double> r;
	sca_tdf::sca_signal<double> e;
	sca_tdf::sca_signal<double> k;
	sca_tdf::sca_signal<double> y;
	
	
	
};

#endif //SYSTEM_HH
