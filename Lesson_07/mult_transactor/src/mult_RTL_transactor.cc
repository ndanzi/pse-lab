#include "mult_RTL_transactor.hh"


// INTERFACE SIDE:
//****************
mult_RTL_transactor::mult_RTL_transactor(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
{

  target_socket(*this);

  SC_THREAD(WRITEPROCESS);
	sensitive_pos << clk;
  SC_THREAD(READPROCESS);
	sensitive_pos << clk;

}

void mult_RTL_transactor::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{
  cout<<sc_simulation_time()<<" - "<<name()<<" - b_transport"<<endl;
  wait(0, SC_NS);
  tlm::tlm_command trans_command = trans.get_command();

  switch (trans_command) {

  case tlm::TLM_WRITE_COMMAND:
 
   ioDataStruct = *((iostruct*) trans.get_data_ptr());
   trans.set_response_status(tlm::TLM_OK_RESPONSE);
   begin_write.notify();    
   cout<<"\t write notified"<<endl;
   wait(end_write);    
   break;

  case tlm::TLM_READ_COMMAND:
   ioDataStruct = *((iostruct*) trans.get_data_ptr());
   trans.set_response_status(tlm::TLM_OK_RESPONSE);
   begin_read.notify();
   cout<<"\t read notified"<<endl;
   wait(end_read);
   *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
   break;

  default:
   break;

  }
  cout<<sc_simulation_time()<<" - "<<name()<<" - b_transport ended"<<endl;
}






void mult_RTL_transactor::WRITEPROCESS()
{

while (true) {
    	wait(begin_write);
	cout<<sc_simulation_time()<<" - "<<name()<<" - notify received"<<endl;
	reset_to_RTL.write(1);
	p_Out_data1.write(ioDataStruct.number1);
	p_Out_data2.write(ioDataStruct.number2);
	p_Out_enable.write(1);
	end_write.notify();
	wait();
  }
}


void mult_RTL_transactor::READPROCESS()
{
while (true) {
	wait(begin_read);
	cout<<sc_simulation_time()<<" - "<<name()<<" - notify received"<<endl;
    	while(p_In_enable.read() != 1)
		wait();
	ioDataStruct.result=p_In_data.read();
	end_read.notify(); 
  }

}









void mult_RTL_transactor :: end_of_elaboration(){
  reset();
}

void mult_RTL_transactor :: reset(){
  cout<<sc_simulation_time()<<" - "<<name()<<" - reset"<<endl; 
  reset_to_RTL.write(0);
  p_Out_enable.write(0);
  p_Out_data1.write(0);
  p_Out_data2.write(0);

}











bool mult_RTL_transactor::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}



tlm::tlm_sync_enum mult_RTL_transactor::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
 
  return tlm::TLM_UPDATED;
  
}



unsigned int mult_RTL_transactor::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}

