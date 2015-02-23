#include "controller_error.hh"

error::error(sc_core::sc_module_name nm) :
        r("r"), y("y"), e("e") {
    // use a carrier data rate of 40
}

void error::set_attributes() {
    //in_wav.set_rate(rate);
    //out.set_rate(1);
    e.set_delay(1.0);// perchè ?!?!!?
}

void error::initialize(){
	e.initialize(0.0);
}
void error::processing() {
	std::cout<< "CONTROLLER ERROR - READ" << r.read() <<std::endl;
    e.write(( r.read() - y.read() ), 0 );
    std::cout<< "CONTROLLER ERROR - WRITE" << ( r.read() - y.read() ) <<std::endl;
}
