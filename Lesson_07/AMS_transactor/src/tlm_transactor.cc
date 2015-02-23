#include "tlm_transactor.hh"


// INTERFACE SIDE:
//****************
tlm_2_rtl::tlm_2_rtl(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
{

  target_socket(*this);

  SC_THREAD(WRITEPROCESS);
  SC_THREAD(READPROCESS);

}

void tlm_2_rtl::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{
  std::cout<<"TLM_transactor - " << sc_simulation_time()<<" - "<<name()<<" - b_transport"<<std::endl;
  wait(0, SC_NS);
  tlm::tlm_command trans_command = trans.get_command();

  switch (trans_command) {

  case tlm::TLM_WRITE_COMMAND:
 
	   ioDataStruct = *((iostruct*) trans.get_data_ptr());
	   trans.set_response_status(tlm::TLM_OK_RESPONSE);
	   begin_write.notify();    
	   std::cout<<"TLM_transactor - write notified"<<std::endl;
	   wait(end_write);    
  break;

  case tlm::TLM_READ_COMMAND:
	   ioDataStruct = *((iostruct*) trans.get_data_ptr());
	   trans.set_response_status(tlm::TLM_OK_RESPONSE);
	   begin_read.notify();
	   std::cout<<"TLM_transactor - read notified"<<std::endl;
	   wait(end_read);
	   *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  break;

  default:
   break;

  }
  std::cout<<"TLM_transactor - "<< sc_simulation_time()<<" - "<<name()<<" - b_transport ended"<<std::endl;
}






void tlm_2_rtl::WRITEPROCESS()
{

while (true) {
    
    wait( begin_write );
    
    //pass the value from the  iostruct to the RTL port side
    
    r.write(ioDataStruct.r); 
    std::cout<<"TLM_transactor - WRITE: "<< ioDataStruct.r<<std::endl;
	std::cout<<"TLM_transactor - "<< sc_simulation_time() <<" - "<<name()<<" - notify received"<<std::endl;

	
	end_write.notify();
  }
}


void tlm_2_rtl::READPROCESS()
{
while (true) {
	
	wait( begin_read );
	
	// pass the values from the RTL ports to the iodastruct.
	
	ioDataStruct.y=y.read();
	
	std::cout << "TLM_transactor - READ: "<< y.read() << std::endl;
	
	std::cout<<"TLM_transactor - "<< sc_simulation_time()<<" - "<<name()<<" - notify received"<<std::endl;
	
	end_read.notify(); 
  }

}




void tlm_2_rtl :: end_of_elaboration(){
  reset();
}

void tlm_2_rtl :: reset(){


}





bool tlm_2_rtl::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}



tlm::tlm_sync_enum tlm_2_rtl::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
 
  return tlm::TLM_UPDATED;
  
}



unsigned int tlm_2_rtl::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}
