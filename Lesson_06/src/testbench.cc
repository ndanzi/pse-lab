#include "testbench.h"

#include <iostream>
#include <fstream>

#define INPUT_FILE "inputs.txt"

using namespace std;

testbench::testbench(sc_module_name name_)
: sc_module(name_)
{
	SC_THREAD(run);
	sensitive << clk.pos();
	SC_THREAD(clk_gen);
}

void testbench::clk_gen()
{
	while( true )
	{
		clk.write( sc_dt::SC_LOGIC_1 );
		wait(PERIOD / 2, sc_core::SC_MS);
		clk.write( sc_dt::SC_LOGIC_0 );
		wait(PERIOD / 2, sc_core::SC_MS);
	}
}

void testbench::run()
{

    std::fstream myfile(INPUT_FILE, std::ios_base::in);

    double a;

    cout << "\ttb: reading file "<< INPUT_FILE << endl;
    if (!myfile)
    {
        cout << "\ttb: error!"<< endl;
    }
    while (myfile >> a)
    {
        number.write(a);
        cout << "\ttb: sent " << a << " to Controller" << endl;
        wait();
    }

    sc_stop();

}


