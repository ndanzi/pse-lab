#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <systemc-ams>

SCA_TDF_MODULE(controller) {
    public:
        // input port baseband signal
        sca_tdf::sca_in<double> e;
        
        // output port modulated signal
        sca_tdf::sca_out<double> k;

        controller(sc_core::sc_module_name nm);

        void set_attributes();

		void initialize();
		
        void processing();

    private:
    
    double e_prec;
    double k_prec;
    double k_calc;
};

#endif
