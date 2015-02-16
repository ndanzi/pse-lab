#include "mult_AT4_testbench.hh"

void mult_AT4_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mult_AT4_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  
  // There must be a pending response.
  if (!response_pending) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  // The phase should be BEGIN_RESP.
  if (phase != tlm::BEGIN_RESP) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }
  available_response.notify();
  phase = tlm::END_RESP;

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

void mult_AT4_testbench::run()
{
  sc_time local_time = SC_ZERO_TIME;

  // First transaction (initialization)
  iostruct mult_packet;
  tlm::tlm_generic_payload payload;	
  double n1, n2, result_mult;

  cout<<"Calculate the multiplication of 128 couples of random real numbers!"<<endl;

  
  srand(time(NULL));
  for (int i = 1; i <= 128; i++){
    
    n1 = (rand() % 256000 - 128000) / 1000.0;
    n2 = (rand() % 256000 - 128000) / 1000.0;
    mult_packet.number1 = doubleToLogicVector(n1);
    mult_packet.number2 = doubleToLogicVector(n2);
    cout << "\tnumber1:\t" << n1 << endl;
    cout << "\tnumber2:\t" << n2 << endl;

    // start write transaction
    tlm::tlm_phase phase = tlm::BEGIN_REQ;
    tlm::tlm_generic_payload payload;
    payload.set_address(0);
    payload.set_data_ptr((unsigned char*) &mult_packet);
    payload.set_write();

    tlm::tlm_sync_enum result = initiator_socket->nb_transport_fw(payload, phase, local_time);
    
    if (result == tlm::TLM_COMPLETED) {
  
      // If the target immediately completes the transaction something
      // wrong happened. We should inspect the response status and
      // take appropriate actions. In this case we just stop the simulation.
      sc_stop();
    }

    // Phase must be END_REQ
    if (phase != tlm::END_REQ) {

      sc_stop();
    }

    response_pending = true;
    wait(available_response);
    response_pending = false;

    // start read transaction
    phase = tlm::BEGIN_REQ;
    payload.set_address(0);
    payload.set_data_ptr((unsigned char*) &mult_packet);
    payload.set_read();

    result = initiator_socket->nb_transport_fw(payload, phase, local_time);

    if (result == tlm::TLM_COMPLETED) {

      // If the target immediately completes the transaction something
      // wrong happened. We should inspect the response status and
      // take appropriate actions. In this case we just stop the simulation.
      sc_stop();
    }

    // Phase must be END_REQ
    if (phase != tlm::END_REQ) {

      sc_stop();
    }

    response_pending = true;
    wait(available_response);
    response_pending = false;    

    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      result_mult = logicVectorToDouble(mult_packet.result);
      cout << "\tresult:\t" << result_mult << endl;
      cout<<endl;
    }
      
  }
  sc_stop();
    
}







mult_AT4_testbench::mult_AT4_testbench(sc_module_name name)
  : sc_module(name)
  , response_pending(false)
{

  initiator_socket(*this);

  SC_THREAD(run);

}
