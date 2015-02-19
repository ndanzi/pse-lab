#ifndef TESTBENCH_H_
#define TESTBENCH_H_

#define PERIOD 40

#include <systemc.h>
#include <systemc-ams.h>

class testbench : public sc_core::sc_module
{

private:

	void run();
	void clk_gen();

public:

	sc_out< double >    number;
	sc_out< sc_logic > 	clk;

	SC_HAS_PROCESS(testbench);
	testbench(sc_module_name name);

};


#endif /* TESTBENCH_H_ */
