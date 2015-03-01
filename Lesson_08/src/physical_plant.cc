
#include "physical_plant.hh"
//costructor
plant::plant(sc_core::sc_module_name) :
        k("k"), tdf2lsf("tdf2lsf"), lsf2tdf("lsf2tdf"), y("y"),   ltf_nd("ltf_nd"), sig1("sig1"), sig2("sig2") {

	// transform function = (1)/(13s+s^2)
	//coefficient, numerator,num ,  and denominator (den)
	num(0)	= 	1.0;
	den(0)	=	0.0;
	den(1)	=	13;
	den(2)	=	1.0;
	
  /* TDF Input Ports. */
  tdf2lsf.inp(k);
  tdf2lsf.y(sig1);
	
	ltf_nd.num = num;
	ltf_nd.den = den;
	ltf_nd.x(sig1);

	ltf_nd.y(sig2);
	
	/* TDF Output Ports. */
  lsf2tdf.x(sig2);
  lsf2tdf.outp(y);
    
}

plant::~plant() {

}
