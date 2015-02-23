#include "rtl_transactor.hh"

rtl_2_tdf::rtl_2_tdf(sc_core::sc_module_name nm) :
        r_rtl("r_rtl"), y_tdf("y_tdf"), r_tdf("r_tdf"), y_rtl("y_rtl") {

}

void rtl_2_tdf::set_attributes() {
 
    set_timestep( 20.0, sc_core::SC_MS); 
}

void rtl_2_tdf::initialize(){
	
	
}
void rtl_2_tdf::processing() {
    std::cout<<"RTL_transactor - READ: "<<r_rtl.read()<< std::endl;

    r_tdf.write( r_rtl.read() );
    std::cout<<"RTL_transactor - WRITE: "<<y_tdf.read()<< std::endl;

    y_rtl.write( y_tdf.read() );
}
