#ifndef __tlm_2_rtl_HPP__
#define __tlm_2_rtl_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_TLM.hh"

class tlm_2_rtl
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


	

  //input for testbench. result from mult module
  sc_core::sc_in<sc_lv<64> >     result;
  //input for testbench. result is ready from mult module
  sc_core::sc_in<sc_uint<1> >  	result_is_ready;
  //input for testbench. clock from mult module
  sc_core::sc_in<sc_logic >     clk;
  //output for mult. number1 
  sc_core::sc_out<sc_lv<64> >     number1;
  //output for mult. number2 
  sc_core::sc_out<sc_lv<64> >     number2;
  //output for mult. numbers are ready 
  sc_core::sc_out<sc_uint<1> >    numbers_are_ready;
  //output for mult. reset
  sc_out<bool > 		    reset_to_rtl;

  sc_signal<bool > rst;
  

  //processes
  void WRITEPROCESS();
  void READPROCESS();

  //mandatory for TLM
  void sync();
  void end_of_elaboration();
  void reset();
  
  
  SC_HAS_PROCESS(tlm_2_rtl);
  tlm_2_rtl(sc_module_name name_);
  
};

#endif





