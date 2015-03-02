#include "testbench_TLM.hh"


// constructor to initialize the TLM socket and the main thread
testbench_TLM::testbench_TLM(sc_module_name name)
  : sc_module(name)
{

  initiator_socket(*this);
  SC_THREAD(run);
  
  //OPERATOR 1
	std::ifstream ifile("log/reference.log", std::ios::in);
    

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        std::cerr << "tb - There was a problem opening the input file!\n";
        exit(1);//exit or do additional error checking
    }

    double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        testbench_TLM::input_reference.push_back(num);
    }

	
	

}



void testbench_TLM::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum testbench_TLM::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}


void testbench_TLM::run()
{

  sc_core::sc_time local_time;
  iostruct packet;
  tlm::tlm_generic_payload payload;	

  local_time = m_qk.get_local_time();
  std::cout << "tb - size :"<<sc_simulation_time()<<" - "<<name()<<" - run"<<std::endl;
  for(uint32_t i=0; i< input_reference.size(); i++)
  {
		packet.r = testbench_TLM::input_reference[i] ;
		
		payload.set_data_ptr((unsigned char*) &packet); // set payload data
		payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator 
		payload.set_write(); // write transaction
		
		// update the local time variable to send it to the target
		local_time = m_qk.get_local_time();
		std::cout<<"tb - WRITING "<<packet.r <<std::endl;
		std::cout<<"tb - " << i <<" Invoking the b_transport primitive - write"<<std::endl;
		initiator_socket->b_transport(payload, local_time); // invoke the transport primitive
		
		
		// start read transaction
		payload.set_read();
		initiator_socket->b_transport(payload, local_time);
		
		
		if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
		// check that the protocol has been correctly implemented
		// and print the result
		std::cout<<"tb - TLM protocol correctly implemented"<<std::endl;
		std::cout<<"tb - READING " << packet.y << std::endl;
		}
		
		
		std::cout << "tb - Time: " << sc_time_stamp() << " + " << local_time << std::endl;
		m_qk.set(local_time);
		if (m_qk.need_sync()) {
			// synchronize simulation time
			cout << "SYNCHRONIZING" << endl;
			m_qk.sync();
			cout << "#####################" << endl;
		}
	  wait(40,SC_MS);
  }
  // send one random number - write invocation
  
  
  
  sc_stop();
  
}
