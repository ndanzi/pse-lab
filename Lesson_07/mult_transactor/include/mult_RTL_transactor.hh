#ifndef __mult_RTL_transactor_HPP__
#define __mult_RTL_transactor_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_LT.hh"

class mult_RTL_transactor
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


  //RTL side
  
  //RTL ports
  sc_in<sc_uint<1> >  	p_In_enable;
  sc_in<sc_lv<64> > 	  p_In_data;
  sc_out<sc_uint<1> >  	p_Out_enable;
  sc_out<sc_lv<64> > 	  p_Out_data1;
  sc_out<sc_lv<64> > 	  p_Out_data2;
  sc_out< bool > 		    reset_to_RTL;
  sc_in< sc_logic >            	clk;
  sc_signal<bool > rst;

  //processes
  void WRITEPROCESS();
  void READPROCESS();

  //mandatory for TLM
  void sync();
  void end_of_elaboration();
  void reset();
  
  SC_HAS_PROCESS(mult_RTL_transactor);
  mult_RTL_transactor(sc_module_name name_);
  
};

#endif





