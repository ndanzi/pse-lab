#include "controller_error.h"

void controller_error::set_attributes (){

	set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
	log.open("log/execution.log", ios::trunc);
	ref.open("log/reference.log", ios::trunc);

}


void controller_error::initialize(){

	k_calc_pre = 0;
	err_calc_pre = 0;

}


void controller_error::processing (){

	r_calc = r_input.read();
	y_calc = y_input.read();
  //cout << "c_err: read " << r_calc << " on r_in" << endl;
  //cout << "c_err: read " << y_calc << " on y_in" << endl;
	err_calc = r_calc - y_calc;
	err_out.write(err_calc);
  //cout << "c_err: write " << err_calc << " on err_out" << endl;
  log << sc_time_stamp().to_seconds() << " " << y_calc << endl;
  ref << sc_time_stamp().to_seconds() << " " << r_calc << endl;
	
}
