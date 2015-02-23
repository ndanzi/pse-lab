#include "mult_LT_testbench.hh"

void mult_LT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mult_LT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  cout<<sc_simulation_time()<<" - "<<name()<<" - nb_transport_bw"<<endl;
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

void mult_LT_testbench::run()
{

  sc_time local_time = m_qk.get_local_time();

  // First transaction (initialization)
  iostruct mult_packet;
  tlm::tlm_generic_payload payload;	

  sc_lv<64> temp_data1_in, temp_data2_in, result_lv = doubleToLogicVector( 0 );
  double result = 0;
  double n1, n2, proof_result;
  static sc_lv<64> proof_result_bin;
  bool right_multiplication = true;

  srand(time(NULL));

//  for (int i = 1; i <= 128; i++){
    cout<<sc_simulation_time()<<" - "<<name()<<" - run"<<endl;

    n1 = (rand() % 256000 - 128000) / 1000.0;
    n2 = (rand() % 256000 - 128000) / 1000.0;

    mult_packet.number1 = doubleToLogicVector(n1);
    mult_packet.number2 = doubleToLogicVector(n2);
    proof_result = n1 * n2;
    proof_result_bin = doubleToLogicVector(proof_result);

    cout<<"\tThe multiplication between "<< n1 << " and " << n2 << endl;
    payload.set_data_ptr((unsigned char*) &mult_packet);
    payload.set_address(0);
    payload.set_write();
    
    local_time = m_qk.get_local_time();

    // start write transaction
    initiator_socket->b_transport(payload, local_time);

    // start read transaction
    payload.set_read();
    initiator_socket->b_transport(payload, local_time);
    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      result = logicVectorToDouble(mult_packet.result);
      cout<< "... is: " << result << endl;
    }

    cout << "Time: " << sc_time_stamp() << " + " << local_time << endl;
    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      cout << "SYNCHRONIZING" << endl;
      m_qk.sync();
      cout << "#####################" << endl;
    }
    
//  }
  sc_stop();
  
}

mult_LT_testbench::mult_LT_testbench(sc_module_name name)
  : sc_module(name)
{

  initiator_socket(*this);

  SC_THREAD(run);

  m_qk.set_global_quantum(sc_time(500, SC_NS));
  m_qk.reset();

}
