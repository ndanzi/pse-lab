#include "mult_UT_testbench.hh"

void mult_UT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mult_UT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
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


void mult_UT_testbench::run()
{

  sc_time local_time;
  iostruct mult_packet;
  tlm::tlm_generic_payload payload;	
  double n1, n2, result;	

  // send two random numbers - write invocation
  srand(time(NULL));
  n1 = (rand() % 256000 - 128000) / 1000.0;
  n2 = (rand() % 256000 - 128000) / 1000.0;
  mult_packet.number1 = doubleToLogicVector(n1);
  mult_packet.number2 = doubleToLogicVector(n2);
  //cout << "\tnumber1:\t" << n1 << endl;
  //cout << "\tnumber2:\t" << n2 << endl;

  cout<< "\t" <<sc_time_stamp()<< " - [TB:] Calculating the product between "<<n1<<" and "<<n2<<endl;
  payload.set_data_ptr((unsigned char*) &mult_packet); // set payload data
  payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator 
  payload.set_write(); // write transaction

  cout<< "\t" <<sc_time_stamp()<< " - [TB:] Invoking the b_transport primitive - write"<<endl;
  initiator_socket->b_transport(payload, local_time); // invoke the transport primitive

  if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
    // check that the protocol has been correctly implemented
    // and print the result
    cout<< "\t" <<sc_time_stamp()<< " - [TB:] TLM protocol correctly implemented"<<endl;
    result = logicVectorToDouble(mult_packet.result);
    cout<< "\t" <<sc_time_stamp()<< " - [TB:] Result is: " << result << endl;
  }
  sc_stop();
  
}

// constructor to initialize the TLM socket and the main thread
mult_UT_testbench::mult_UT_testbench(sc_module_name name)
  : sc_module(name)
{

  initiator_socket(*this);
  SC_THREAD(run);

}
