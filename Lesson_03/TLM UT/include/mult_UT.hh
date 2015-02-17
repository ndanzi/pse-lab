#ifndef __mult_UT_HPP__
#define __mult_UT_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_UT.hh"

class mult_UT
  : public sc_module
  , public virtual tlm::tlm_fw_transport_if<>
{

 public:

  // TLM port - target interface
  tlm::tlm_target_socket<> target_socket;

  // function invoked by the initiator
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);

  // functions that must be implemented to be compliant with the standard
  // not used here
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  // internal data
  iostruct  ioDataStruct;
  tlm::tlm_generic_payload* pending_transaction;
  sc_lv<64> tmp_result;

  // functionality  
  void mult_function();

  // constructors
  SC_HAS_PROCESS(mult_UT);
  mult_UT(sc_module_name name_);
  
};

#endif

