#ifndef __root_AT4_HPP__
#define __root_AT4_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_AT4.hh"

class root_AT4
  : public sc_module
  , public virtual tlm::tlm_fw_transport_if<>
{

 public:

  // TLM socket 
  tlm::tlm_target_socket<> target_socket;

  // must be implemented to be compliant with the standard
  // not used here
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);
  
  // non blocking transport interface
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);

  iostruct  ioDataStruct;
  tlm::tlm_generic_payload* pending_transaction;
  sc_event io_event;

  // target process that performs elaboration when awakened by the initiator
  void IOPROCESS();

  // elaboration function
  void root_function();
  
  SC_HAS_PROCESS(root_AT4);

  root_AT4(sc_module_name name_);
  
};

#endif

