#include "controller_error.hh"

error::error(sc_core::sc_module_name nm) :
        r("r"), y("y"), e("e") {

}

void error::set_attributes() {

  set_timestep( 20.0, sc_core::SC_MS);
    e.set_delay(1.0);
}

void error::initialize(){
	e.initialize(0.0);
}
void error::processing() {
	std::cout<< "controller error - READ: " << r.read() <<std::endl;
    e.write(( r.read() - y.read() ), 0 );
    std::cout<< "controller error - WRITE: " << ( r.read() - y.read() ) <<std::endl;
}
