#include "testbench_TLM.hh"
#include <math.h>      

#define PERIOD 100000

#define PI 3.14159265

void testbench_TLM::clk_gen()
{
  while( true )
  {
    clk.write( sc_dt::SC_LOGIC_1 );
    wait( PERIOD / 2, sc_core::SC_NS );
    clk.write( sc_dt::SC_LOGIC_0 );
    wait( PERIOD / 2, sc_core::SC_NS );   
  }
}

// constructor to initialize the TLM socket and the main thread
testbench_TLM::testbench_TLM(sc_module_name name)
  : sc_module(name)
{

  initiator_socket1(*this);
  initiator_socket2(*this);
  SC_THREAD(run1);
	sensitive << clk.pos();
  SC_THREAD( clk_gen );
  
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

  m_qk.set_global_quantum(sc_time(500, SC_NS));
  m_qk.reset();

  srand(time(NULL));

  for (int i = 1; i <= 360; i++){

    testbench_TLM::n1.push_back(i*sin(i*PI/180.0));
    testbench_TLM::n2.push_back(1.0/i);

  }

}



void testbench_TLM::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum testbench_TLM::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}

//functions to convert lv to double or viceversa
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
  iostruct plant_packet;
  tlm::tlm_generic_payload plant_payload;	
  int delay = 0;
	ofstream log;
	log.open("log/execution.log", ios::trunc);

  local_time = m_qk.get_local_time();
  //std::cout << "tb - run1 - size :"<<sc_simulation_time()<<" - "<<name()<<" - run"<<std::endl;

  for(int i = 0; i < 360; i++) {
    mult_packet.number1 = doubleToLogicVector(testbench_TLM::n1[i]);
    mult_packet.number2 = doubleToLogicVector(testbench_TLM::n2[i]);

    cout<<"\t\t\tThe multiplication n " << i <<" multiplication between "<< testbench_TLM::n1[i] << " and " << testbench_TLM::n2[i] << endl;
    mult_payload.set_data_ptr((unsigned char*) &mult_packet);
    mult_payload.set_address(0);
    mult_payload.set_write();
    
    local_time = m_qk.get_local_time();

    // start write transaction
    initiator_socket1->b_transport(mult_payload, local_time);

    // start read transaction mult
    mult_payload.set_read();
    initiator_socket1->b_transport(mult_payload, local_time);


    if(mult_payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      testbench_TLM::results.push_back(logicVectorToDouble(mult_packet.result));
      cout<< "\t\t\tresult of multiplication n " << i << "... is: " << testbench_TLM::results[i] << endl;
	    //mult_provided.notify(); 
    }


    // start read transaction plant
    plant_payload.set_data_ptr((unsigned char*) &plant_packet);
    plant_payload.set_address(0);
    plant_payload.set_write();

    local_time = m_qk.get_local_time();
    //cout << "TLM : got local time" << endl;

    initiator_socket2->b_transport(plant_payload, local_time);
    plant_payload.set_read();
    initiator_socket2->b_transport(plant_payload, local_time);




    if(plant_payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      cout<< "y: " << plant_packet.y << endl;
      if(plant_packet.y == 0) {
        delay++;
        cout << "delay = " << delay << endl;
      } else {
        cout << "result = " << testbench_TLM::results[i - delay] << endl;
        cout << "threshold = " << (testbench_TLM::results[i - delay] - plant_packet.y)/testbench_TLM::results[i - delay] << endl;
        log << i << " " << testbench_TLM::results[i - delay] << " " << plant_packet.y << endl;
        if((testbench_TLM::results[i - delay] - plant_packet.y)/testbench_TLM::results[i - delay] > 0.1) {
           std::cout<<"\033[31m"<<"ERROR!!!"<<"\033[0m"<< std::endl;
        }
      }
    }





    //cout << "Time: " << sc_time_stamp() << " + " << local_time << endl;
    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      //cout << "SYNCHRONIZING" << endl;
      m_qk.sync();
      //cout << "#####################" << endl;
    }
    //cout << "TLM : wait 80 ms" << endl;
  	wait(80,SC_MS);
    //cout << "TLM : waited" << endl;
  }
  sc_stop();


  
}


