#ifndef PHYSICAL_PLANT_H_
#define PHYSICAL_PLANT_H_

#include <systemc-ams.h>
#include <systemc.h>

SCA_TDF_MODULE (physical_plant )
{

public:
	sca_tdf::sca_in<double> k_input;

	sca_tdf::sca_out<double> y_out;


	void set_attributes ();
	void initialize();
	void processing ();

	SCA_CTOR(p_plant)
	{
	}

private:

    
   double fc;
   double h0;

	//funzione di trasferimento
	sca_tdf::sca_ltf_nd ltf_nd;
	sca_util::sca_vector<double> num, den; // numerator and denominator coefficients


};

#endif /* PHYSICAL_PLANT_H_ */
