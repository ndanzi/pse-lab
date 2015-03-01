#include "system.hh"

#define TRACE_PLANT
#define TABULAR_FILE "log/outputs"
int sc_main( int ac, char *av[] )
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
	sc_core::sc_set_time_resolution(1.0, sc_core::SC_FS);
	
	System system("system");
	
	#ifdef TRACE_PLANT

		sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file(TABULAR_FILE);
		sca_util::sca_trace(atf, system.r   , "r");
		sca_util::sca_trace(atf, system.y   , "y");
		
	#endif
	
	
	sc_start( );
	return 0;
}
