#include <systemc-ams.h>
#include <systemc.h>
#include "controller.h"
#include "controller_error.h"
#include "physical_plant.h"
#include "testbench.h"


int sc_main(int argc, char* argv[]) {

    sc_core::sc_set_time_resolution(1.0, sc_core::SC_FS);
    sc_signal< sc_logic >         clock;
    sc_signal<double> r_in;
    sca_tdf::sca_signal<double> k_t;
    sca_tdf::sca_signal<double> y_out;
    sca_tdf::sca_signal<double> errore;

    testbench testbench_module ("testbench");
    controller controller_istance("controller");
    controller_error controller_error_istance("controller_error");
    physical_plant	physical_plant_istance("physical_plant");

    testbench_module.number(r_in);
    testbench_module.clk(clock);

    controller_error_istance.r_input(r_in);
    controller_error_istance.y_input(y_out);
    controller_istance.k_out(k_t);
    controller_error_istance.err_out(errore);
    controller_istance.err_in(errore);   

    physical_plant_istance.k_input(k_t);
    physical_plant_istance.y_out(y_out);

    sc_core::sc_start();

    return 0;
}
