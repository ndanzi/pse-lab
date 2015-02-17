#ifndef __mult_LT_HPP__
#define __mult_LT_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_LT.hh"

class mult_LT
  : public sc_module
  , public virtual tlm::tlm_fw_transport_if<>
{

 public:
  
  // TLM socket
  tlm::tlm_target_socket<> target_socket;

  // must be implemented to be complian with the protocol
  // not necessary here
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
  
  void end_of_elaboration(); 
  void reset(); 
  
  // non blocking transport primitive
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);

  iostruct  ioDataStruct;
  tlm::tlm_generic_payload* pending_transaction;
  sc_lv<64> tmp_result;
  sc_time timing_annotation;

  // elaboration function
  void mult_function();

  SC_HAS_PROCESS(mult_LT);

  mult_LT(sc_module_name name_);
  
};

#endif

