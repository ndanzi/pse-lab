#ifndef ERROR_H
#define ERROR_H

#include <systemc-ams>
#include <systemc>
SCA_TDF_MODULE(error) {
    public:
        // input port baseband signal
        sca_tdf::sca_in<double> r;

        // input port carrier signal
        sca_tdf::sca_in<double> y;

        // output port modulated signal
        sca_tdf::sca_out<double> e;

        error(sc_core::sc_module_name nm);

        void initialize();
        
        void set_attributes();
        

        void processing();

    private:
};

#endif
