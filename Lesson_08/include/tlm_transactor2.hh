#ifndef __tlm_2_ams_HPP__
#define __tlm_2_ams_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_TLM.hh"

class tlm_2_ams
  : public sc_module
  , public virtual tlm::tlm_fw_transport_if<>
{

 public:

   //TLM side

  tlm::tlm_target_socket<> target_socket;
  iostruct  ioDataStruct;
  tlm::tlm_generic_payload* pending_transaction;
  sc_time timing_annotation;
  sc_event begin_write, end_write, begin_read, end_read;

  //TLM interfaces
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);


	
	//input for the testbench. result from the error module of the plant
	sc_core::sc_in < double >  y;

	//output for tdf_2_rtl
	sc_core::sc_out< double > r;
  

  //processes
  void WRITEPROCESS();
  void READPROCESS();

  //mandatory for TLM
  void sync();
  void end_of_elaboration();
  void reset();
  
  
  SC_HAS_PROCESS(tlm_2_ams);
  tlm_2_ams(sc_module_name name_);
  
};

#endif





