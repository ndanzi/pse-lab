#include "mult_UT.hh"

// constructor
// initialize the TLM socket
// no process, target works with function invocations from the initiator
mult_UT::mult_UT(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
  , pending_transaction(NULL)
{

  target_socket(*this);

}

void mult_UT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{
  // function invoked by the initiator
  
  // download data from the payload
  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  if (trans.is_write()) {
    cout<<"\t\t[MULT:] Received invocation of the b_transport primitive - write"<<endl;
    cout<<"\t\t[MULT:] Invoking the mult_function to calculate the product"<<endl;

    // start elaboration
    // compute the functionality
    mult_function();
    
    ioDataStruct.result = tmp_result;
    
    // and load it on the payload
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
    cout<<"\t\t[MULT:] Returning result: "<<tmp_result<<endl;
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  } 
  else if (trans.is_read()){
    cout<<"\t\t[MULT:] Received invocation of the b_transport primitive - read"<<endl;

    // return the result
    ioDataStruct.result = tmp_result;

    // and load it on the payload
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
    cout<<"\t\t[MULT:] Returning result: "<<tmp_result<<endl;
  }

}

void mult_UT:: mult_function()
{
  cout<<"\t\t[MULT:] Calculating mult_function ... "<<endl;
  double n1, n2, result;
   
  uint64_t tmp;
  //retrive number 1 and convert it to double
  tmp = (ioDataStruct.number1).to_uint64();
  n1 = *reinterpret_cast< double * >(&tmp);
  //retrive number 2 and convert it to double
  tmp = (ioDataStruct.number2).to_uint64();
  n2 = *reinterpret_cast< double * >(&tmp);

  //perform multiplication
  result = n1 * n2;

  //convert it to logic vector and assign it to tmp_result
  tmp_result = *reinterpret_cast< uint64_t * >(&result);
}




// must be implemented to be compliant with the interface
// not significant in this code

bool mult_UT::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}

tlm::tlm_sync_enum mult_UT::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
  return tlm::TLM_COMPLETED;
}

unsigned int mult_UT::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}
