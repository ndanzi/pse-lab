#include "mult_RTL_testbench.hh"
#include <stdint.h>
#include <limits>

  mult_RTL_testbench::mult_RTL_testbench(sc_module_name name_)
: sc_module(name_)
{

  SC_THREAD(run);
  sensitive << clk.pos();

  SC_THREAD(clk_gen);
}



void mult_RTL_testbench::clk_gen()
{
  while( true )
  {
    clk.write( sc_dt::SC_LOGIC_1 );
    wait(PERIOD / 2, sc_core::SC_NS);
    clk.write( sc_dt::SC_LOGIC_0 );
    wait(PERIOD / 2, sc_core::SC_NS);
  }
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

void mult_RTL_testbench::run()
{

  sc_lv<64> temp_data1_in, temp_data2_in, result_lv = doubleToLogicVector( 0 );
  double result = 0;
  double n1, n2, proof_result;
  static sc_lv<64> proof_result_bin;
  bool right_multiplication = true;

  cout<<"Calculate the multiplication of 128 number!"<<endl;
  srand(time(NULL));
  for (int i = 1; i <= 128; i++){
    
    n1 = (rand() % 256000 - 128000) / 1000.0;
    n2 = (rand() % 256000 - 128000) / 1000.0;
    //n1 = 0.5;
    //n2 = 0.7;
    //int max = std::numeric_limits<int>::max();
    //double min std::numeric_limits<float>::min();
    //n1 = (rand() % max - (max/2)) / 1000.0;
    //n2 = (rand() % max - (max/2)) / 1000.0;
    
    temp_data1_in = doubleToLogicVector(n1);
    temp_data2_in = doubleToLogicVector(n2);
    proof_result = n1 * n2;
    proof_result_bin = doubleToLogicVector(proof_result);

    //cout<<"\tThe multiplication between "<< n1 << " and " << n2 << endl;

    reset_to_RTL.write(1);
    p_Out_data1.write(temp_data1_in);
    p_Out_data2.write(temp_data2_in);

    p_Out_enable.write(1);
    wait();

    while(p_In_enable.read() != 1) wait();
    result_lv = p_In_data.read();
    result = logicVectorToDouble(result_lv);
    //cout << "s: " << result_lv.range(63, 63) << endl;
    //cout << "e: " << result_lv.range(62, 52) <<  endl;
    //cout << "m: " << result_lv.range(51, 0) <<  endl;
    cout<<"The multiplication between "<< n1 << " and " << n2 << endl;
    cout << "\tis: ";
    printf("%.50f\n", result);

    
    
    //cout << "result prooof is:" << endl;
    //printf("%.100f\n", proof_result);
    //cout << proof_result_bin << endl;
    
    
    //cout << "s: " << proof_result_bin.range(63, 63) << endl;
    //cout << "e: " << proof_result_bin.range(62, 52) <<  endl;
    //cout << "m: " << proof_result_bin.range(51, 0) <<  endl;
    
    if(proof_result != result) {
      cout << "\tmultiplication was wrong" << endl;
      right_multiplication = false;
    }else {
      cout << "\tmultiplication was right" << endl;
    }
    

  }
  cout << "\n\n************************************************************************************\n\n" << endl;
  
  if(right_multiplication)
    cout << "\nAll multiplications were right!!" << endl;
  else
    cout << "\nAt least one multiplication went wrong!!" << endl;

  reset_to_RTL.write(0);
  p_Out_enable.write(0);
  p_Out_data1.write(0);
  p_Out_data2.write(0);

  sc_stop();

}


