#include "physical_plant.h"

void physical_plant::set_attributes (){

	set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
	y_out.set_delay(1.0); 						// set delay of port out to 2 samples

}

void physical_plant::initialize(){

	//costruisco la funzione di trasferimento
	num(0) = 1.0;
	den(0) = 0;
	den(1) = 13;
	den(2) = 1;

}

void physical_plant::processing (){

  double ltf_function;
  ltf_function = ltf_nd( num, den, k_input.read());
	y_out.write( ltf_function );
  //cout << "p_plant: write " << ltf_function << " on y_out" << endl;

}
