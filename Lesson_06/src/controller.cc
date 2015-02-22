#include "controller.h"


void controller::set_attributes (){

	set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
 						
}

void controller::initialize(){

	k_calc_pre = 0;
	err_calc_pre = 0;
  k_calc = 0;
  err_calc = 0;

}

void controller::processing (){

	err_calc = err_in.read();

	k_calc = k_calc_pre + (100 * (err_calc - err_calc_pre)) + (get_timestep().to_seconds() * err_calc);
	k_calc_pre = k_calc;
	err_calc_pre = err_calc;

	k_out.write(k_calc);
  //cout << "c: write " << k_calc << " on k_out" << endl;

}
