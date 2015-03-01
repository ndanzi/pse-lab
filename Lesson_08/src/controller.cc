#include "controller.hh"

controller::controller(sc_core::sc_module_name nm) :
        e("e"), k("k"){
}

void controller::set_attributes() {

  // time_step!
  set_timestep( 20.0, sc_core::SC_MS); 
  k_prec = 0.0;
  e_prec = 0.0;

}

void controller::initialize(){
	//output port initialization
	k.initialize(0.0);
}
void controller::processing() {

	//std::cout<<"controller - READ: " << e.read() << std::endl;
	k_calc = k_prec + ( 100 * ( e.read() - e_prec ) ) + ( get_timestep().to_seconds() * e.read() );
	//std::cout<<"controller - WRITE: " << k_calc << std::endl;
	k.write( k_calc );
	e_prec = e.read();
	k_prec = k_calc;
	
}
