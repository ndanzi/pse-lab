#include "testbench_TLM.hh"


// constructor to initialize the TLM socket and the main thread
testbench_TLM::testbench_TLM(sc_module_name name)
  : sc_module(name)
{

  initiator_socket1(*this);
  initiator_socket2(*this);
  SC_THREAD(run1);
  SC_THREAD(run2);
  
  //OPERATOR 1
	/*std::ifstream ifile("log/reference.log", std::ios::in);
    

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
	
	*/

 

  srand(time(NULL));

  for (int i = 1; i <= 128; i++){

    testbench_TLM::n1.push_back((rand() % 256000 - 128000) / 1000.0);
    testbench_TLM::n2.push_back((rand() % 256000 - 128000) / 1000.0);

  }

}



void testbench_TLM::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum testbench_TLM::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}

sc_dt::sc_lv< 64 > doubleToLogicVector( double d )
{
    uint64_t tmp = *reinterpret_cast< uint64_t * >(&d);
    return sc_lv<64L>(tmp);
}

double logicVectorToDouble( sc_dt::sc_lv< 64 > v )
{
    uint64_t tmp = v.to_uint64();
    return (*reinterpret_cast< double * >(&tmp));
}


void testbench_TLM::run1()
{

  sc_core::sc_time local_time;
  iostruct mult_packet;
  tlm::tlm_generic_payload mult_payload;	

  local_time = m_qk.get_local_time();
  std::cout << "tb - run1 - size :"<<sc_simulation_time()<<" - "<<name()<<" - run"<<std::endl;

  for(int i = 0; i < 128; i++) {
    mult_packet.number1 = doubleToLogicVector(testbench_TLM::n1[i]);
    mult_packet.number2 = doubleToLogicVector(testbench_TLM::n2[i]);

    cout<<"\tThe multiplication between "<< testbench_TLM::n1[i] << " and " << testbench_TLM::n2[i] << endl;
    mult_payload.set_data_ptr((unsigned char*) &mult_packet);
    mult_payload.set_address(0);
    mult_payload.set_write();
    
    local_time = m_qk.get_local_time();

    // start write transaction
    initiator_socket1->b_transport(mult_payload, local_time);

    // start read transaction
    mult_payload.set_read();
    initiator_socket1->b_transport(mult_payload, local_time);


    if(mult_payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      testbench_TLM::results.push_back(logicVectorToDouble(mult_packet.result));
      cout<< "... is: " << testbench_TLM::results[i] << endl;
	    mult_provided.notify(); 
    }



    cout << "Time: " << sc_time_stamp() << " + " << local_time << endl;
    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      cout << "SYNCHRONIZING" << endl;
      m_qk.sync();
      cout << "#####################" << endl;
    }
  }
  
}
void testbench_TLM::run2()
{

  sc_core::sc_time local_time;
  iostruct packet;
  tlm::tlm_generic_payload payload;

  local_time = m_qk.get_local_time();
  std::cout << "tb - run2 - size :"<<sc_simulation_time()<<" - "<<name()<<" - run"<<std::endl;
	
  for(int i = 0; i < 128; i++)
  {
    wait(mult_provided);
		packet.r = results[i] ;
		std::cout << "tb - size :"<<sc_simulation_time()<<" - "<<name()<<" - run"<<std::endl;
		payload.set_data_ptr((unsigned char*) &packet); // set payload data
		payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator 
		payload.set_write(); // write transaction
		
		// update the local time variable to send it to the target
		local_time = m_qk.get_local_time();
		std::cout<<"tb - WRITING "<<packet.r <<std::endl;
		std::cout<<"tb - " << i <<" Invoking the b_transport primitive - write"<<std::endl;
		initiator_socket2->b_transport(payload, local_time); // invoke the transport primitive
		
		
		// start read transaction
		payload.set_read();
    initiator_socket2->b_transport(payload, local_time);
		
		
    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      cout<< "y: " << packet.y << endl;
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

