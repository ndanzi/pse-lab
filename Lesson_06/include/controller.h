#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <systemc-ams.h>

#include <systemc.h>

SCA_TDF_MODULE (controller )
{

public:

	sca_tdf::sca_out<double> k_out;
	sca_tdf::sca_in<double> err_in;

	double r_calc;
	double y_calc;

	double err_calc;
	double err_calc_pre;

	double k_calc;
	double k_calc_pre;

	ofstream log;

	void set_attributes ();
	void initialize();
	void processing ();

	SCA_CTOR(controller)
	{
	}

};

#endif /* CONTROLLER_H_ */
