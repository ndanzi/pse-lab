#include "rtl_transactor.hh"

rtl_2_tdf::rtl_2_tdf(sc_core::sc_module_name nm) :
        r_rtl("r_rtl"), y_tdf("y_tdf"), r_tdf("r_tdf"), y_rtl("y_rtl") {

}

void rtl_2_tdf::set_attributes() {
 
    set_timestep( 20.0, sc_core::SC_MS); 
    //r_rtl.set_rate(128);
    r_rtl.set_delay(1.0);
}

void rtl_2_tdf::initialize(){
	
	  r_rtl.initialize(0);
	
}
void rtl_2_tdf::processing() {
    //std::cout<<"\033[31m"<<"RTL_transactor - READ: "<<r_rtl.read()<<"\033[0m"<< std::endl;

    //translate lv result of mult to double for tdf
    sc_dt::sc_lv<64> r_lv = r_rtl.read();
    uint64_t tmp = r_lv.to_uint64();
    double r_double = *reinterpret_cast< double * >(&tmp);
    //std::cout<<"\033[31m"<<"RTL_transactor - converted to: "<<r_double<<"\033[0m"<< std::endl;

    r_tdf.write( r_double );
    //std::cout<<"RTL_transactor - WRITE: "<<y_tdf.read()<< std::endl;

    y_rtl.write( y_tdf.read() );
}
