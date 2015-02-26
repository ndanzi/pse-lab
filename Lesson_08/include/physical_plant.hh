
#ifndef PLANT_H
#define PLANT_H

#include <systemc-ams>

SC_MODULE(plant) {
    public:
        //--------------------Ports for TDF-----------------------------
        /* TDF Input Ports. */
        //tdf port
        sca_tdf::sca_in<double> k;
        //converter
        sca_lsf::sca_tdf::sca_source tdf2lsf;


        /* TDF Output Ports. */
        //converter
        sca_lsf::sca_tdf::sca_sink lsf2tdf;
        //tdf port
        sca_tdf::sca_out<double> y;

        //--------------------Local Declaration Here--------------------
        
        //La Place transformation numerator/ denum form.
        sca_lsf::sca_ltf_nd ltf_nd;
        
        plant(sc_core::sc_module_name);

        ~plant();

    private:
        sca_lsf::sca_signal sig1,sig2;
        sca_util::sca_vector<double> num;
		sca_util::sca_vector<double> den;
};

#endif

