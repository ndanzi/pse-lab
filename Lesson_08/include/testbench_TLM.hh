#ifndef _testbench_TLM_H_
#define _testbench_TLM_H_

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>
#include "define_TLM.hh"

class testbench_TLM
  : public sc_module
  , public virtual tlm::tlm_bw_transport_if<>
{

private:
	
	SC_HAS_PROCESS(testbench_TLM);
	
	// functions necessary to be compliant with the TLM standard
	// not used here
	virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range);
	virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t);
	
	//quantum keeper
	tlm_utils::tlm_quantumkeeper m_qk;
	
	// thread code
	void run1();
	void run2();

public:

  // TLM socket / initiator interface with mult module
  tlm::tlm_initiator_socket<> initiator_socket1;
  // TLM socket / initiator interface with ams module
  tlm::tlm_initiator_socket<> initiator_socket2;
  //std::vector<double> input_reference;
  std::vector<double> n1;
  std::vector<double> n2;
  std::vector<double> results;

  sc_event mult_provided;
  
  // constructor
  testbench_TLM(sc_module_name name);

};

#endif
