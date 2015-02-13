#include "mult_UT.hh"

mult_UT::mult_UT(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
  , pending_transaction(NULL)
{

  target_socket(*this);

}

void mult_UT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{
  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  if (trans.is_write()) {
    mult_function();
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  } 
  else if (trans.is_read()){
    ioDataStruct.result = tmp_result;
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  }

}

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


void mult_UT:: mult_function()
{
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


// Initialization:
void mult_UT:: end_of_elaboration()
{

}

void mult_UT:: reset()
{

}
