#include "mult_LT.hh"

mult_LT::mult_LT(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
  , pending_transaction(NULL)
{
  target_socket(*this);
}

void mult_LT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{

  timing_annotation = SC_ZERO_TIME;

  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  if (trans.is_write()) {
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Received invocation of the b_transport primitive - write"<<endl;
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Invoking the mult_function to calculate the product"<<endl;
    mult_function();
    ioDataStruct.result = tmp_result;
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Returning result: "<<tmp_result<<endl;
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  } 
  else if (trans.is_read()){
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Received invocation of the b_transport primitive - read"<<endl;
    ioDataStruct.result = tmp_result;
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Returning result: "<<tmp_result<<endl;
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  }

  t += timing_annotation;

}

bool mult_LT::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}

tlm::tlm_sync_enum mult_LT::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
  return tlm::TLM_COMPLETED;
}

unsigned int mult_LT::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}

void mult_LT:: mult_function()
{
  cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Calculating mult_function ... "<<endl;
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
  timing_annotation += sc_time(100, SC_NS);

}


// Initialization:
void mult_LT::end_of_elaboration()
{

}

void mult_LT::reset()
{

}
