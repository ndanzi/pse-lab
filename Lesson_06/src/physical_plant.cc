#include "physical_plant.h"

void physical_plant::set_attributes (){

	y_out.set_delay(1.0); 						// set delay of port out to 2 samples

}

void physical_plant::initialize(){

	//costruisco la funzione di trasferimento
	num(0) = 1.0;
	den(0) = 0;
	den(1) = 13;
	den(2) = 1;

}

void physical_plant::processing (){

	y_out.write( ltf_nd( num, den, k_input.read(), h0 ) );

}
