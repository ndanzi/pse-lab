#include "controller_error.h"

void controller_error::set_attributes (){

	set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
	err_out.set_delay(1.0); 						// set delay of port out to 2 samples

}


void controller_error::initialize(){

	k_calc_pre = 0;
	err_calc_pre = 0;

}


void controller_error::processing (){

	r_calc = r_input.read();
	y_calc = y_input.read();
	err_calc = r_calc - y_calc;
	err_out.write(err_calc);
	
}
