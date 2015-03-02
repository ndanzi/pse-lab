#include "mult_AT4.hh"

// transport function invoked by the initiator
tlm::tlm_sync_enum mult_AT4::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
  // If there is already a pending transaction, we refuse to
  // serve the request because something went wrong
  if (pending_transaction != NULL) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  // Phase must be BEGIN_REQ (begin request), otherwise we set an
  // error response and complete the transaction.
  if (phase != tlm::BEGIN_REQ) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Received invocation of the nb_transport_fw primitive"<<endl;
  cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Activating the IOPROCESS"<<endl;
  pending_transaction = &trans; // download the payload
  ioDataStruct = *((iostruct*) trans.get_data_ptr()); // get the data

  phase = tlm::END_REQ; // pahse is end request now, target accepted to process the data

  // activate the ioprocess process to elaborate data and to 
  io_event.notify();
  
  // return control 
  cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] End of the nb_transport_fw primitive"<<endl;
  return tlm::TLM_UPDATED;
  
}


void mult_AT4::IOPROCESS()
{

  sc_time timing_annotation;

  while (true) {
    wait(io_event);
    // if I am here, then the initiator has invoked the forward transport primitive to issue a request 
    
    cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] IOPROCESS has been activated"<<endl;
    
    // Wait 100ns to model the delay required to
    // process the request - simulate advamncement of time

    wait(100, SC_NS);    

    if (pending_transaction->is_write()) {
      // write request: elaborate the square mult and return
      cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Invoking the mult_function to calculate the product"<<endl;
      mult_function();
    }
    
    // read transaction: return the result to the initiator
    else cout<< "\t\t" <<sc_time_stamp()<< " - [MULT:] Returning result: "<<ioDataStruct.result<<endl;
    
    // transaction went on correctly
    pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);
    
    // upload data and set phase to begin response
    *((iostruct*) pending_transaction->get_data_ptr()) = ioDataStruct;
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    
    cout<< "\t\t" <<sc_time_stamp()<< " - [TB:] Invoking the nb_transport_bw primitive - write"<<endl;
    target_socket->nb_transport_bw(*pending_transaction, phase, timing_annotation); // transport primitive in the initiator - ends the AT4 protocol

    pending_transaction = NULL;

  }

}

// elaboration function
void mult_AT4:: mult_function()
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

  ioDataStruct.result = *reinterpret_cast< uint64_t * >(&result);
}

// constructor 
// initialize TLM socket and set ioprocess as a thread
mult_AT4::mult_AT4(sc_module_name name_)
: sc_module(name_)
, target_socket("target_socket")
, pending_transaction(NULL)
{
  
  target_socket(*this);
  
  SC_THREAD(IOPROCESS);
  
}


// necessary to be compliant with the standard
// not used here
void mult_AT4::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{
  
}

bool mult_AT4::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}

unsigned int mult_AT4::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}
