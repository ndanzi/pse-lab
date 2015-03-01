#ifndef RTL_2_TDF_H
#define RTL_2_TDF_H

#include <systemc-ams>
#include <systemc>
SCA_TDF_MODULE(rtl_2_tdf) {
    public:
    
        // INPUT
        // port from RTL interface, tlm2rtl RTL Side
        sca_tdf::sca_de::sca_in<sc_dt::sc_lv<64> >		r_rtl;  //from mult_RTL sc module
		    // port from plant
        sca_tdf::sca_in<double>				    y_tdf; // from plant tdf lsf module. converted in tdf input

        //OUTPUT
        //port to error module
        sca_tdf::sca_out<double>			    r_tdf;  // connected to error tdf module
        //port for tlm socket
		    sca_tdf::sca_de::sca_out<double>	y_rtl;  // connected to tlm2rtl sc module
        
        rtl_2_tdf(sc_core::sc_module_name nm);

        void initialize();
 
        void set_attributes();
        
        void processing();

    private:
};

#endif
