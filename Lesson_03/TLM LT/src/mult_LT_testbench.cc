#include "mult_LT_testbench.hh"

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

void mult_LT_testbench::run()
{
  cout<<sc_simulation_time()<<" - "<<name()<<" - run"<<endl;
  sc_time local_time = m_qk.get_local_time();

  iostruct mult_packet;
  tlm::tlm_generic_payload payload;	
  double n1, n2, result;	

  srand(time(NULL));
  // send two random numbers - write invocation
  n1 = (rand() % 256000 - 128000) / 1000.0;
  n2 = (rand() % 256000 - 128000) / 1000.0;
  mult_packet.number1 = doubleToLogicVector(n1);
  mult_packet.number2 = doubleToLogicVector(n2);
  cout << "\tnumber1:\t" << n1 << endl;
  cout << "\tnumber2:\t" << n2 << endl;

  cout<< "\t" <<sc_time_stamp()<< " - [TB:] Calculating the product between "<<n1<<" and "<<n2<<endl;
  payload.set_data_ptr((unsigned char*) &mult_packet);
  payload.set_address(0);
  payload.set_write();
  
  // update the local time variable to send it to the target
  local_time = m_qk.get_local_time();

  cout<< "\t" <<sc_time_stamp()<< " - [TB:] Invoking the b_transport primitive - write"<<endl;                         
  initiator_socket->b_transport(payload, local_time); // invoke the transport primitive
  
    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      // check that the protocol has been correctly implemented
      // and print the result
      cout<< "\t" <<sc_time_stamp()<< " - [TB:] TLM protocol correctly implemented"<<endl;
      result = logicVectorToDouble(mult_packet.result);
      cout<< "\t" <<sc_time_stamp()<< " - [TB:] Result is: " << result << endl;
  }

  // temporal decoupling> get time and check if we have to synchronize with the target 
  cout << "Time: " << sc_time_stamp() << " + " << local_time << endl;
  m_qk.set(local_time);
  if (m_qk.need_sync()) {
    // synchronize simulation time
    cout << "SYNCHRONIZING" << endl;
    m_qk.sync();
    cout << "#####################" << endl;
  }
  sc_stop();
  
}


// constructor
mult_LT_testbench::mult_LT_testbench(sc_module_name name)
  : sc_module(name)
{

  // initialize TLM socket
  initiator_socket(*this);

  // run is a thread
  SC_THREAD(run);

  // set parameters for temporal decoupling
  // one quantum is made of 500 ns
  m_qk.set_global_quantum(sc_time(500, SC_NS));
  m_qk.reset();

}




// necessary to be compliant with the standard but not used here
void mult_LT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mult_LT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}
