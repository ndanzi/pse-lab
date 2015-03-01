#include "system.hh"

#define PERIOD 10


System::System( sc_core::sc_module_name ) :
  testbench("testbench"),
  tlm2rtl("TLM_transactor"),
  tlm2ams("TLM_transactor2"),
  rtl2tdf("RTL_transactor"),
  er("er"),
  ctrl("ctrl"),
  plt("plt"),
  mult("mult")
{

  testbench.clk( clock );

  //TLM transactor from TLM to RTL
  //OUT
  tlm2rtl.number1( number1 );
  tlm2rtl.number2( number2 );
  tlm2rtl.numbers_are_ready( numbers_are_ready );
  tlm2rtl.reset_to_rtl( reset_signal );
  tlm2rtl.clk( clock );
  //IN
  tlm2rtl.result( result );
  tlm2rtl.result_is_ready( result_is_ready );

  //TLM transactor2 from AMS to TLM
  //IN
  tlm2ams.y( y_rtl );
  //tlm2ams.clk( clock );
  //OUT
  //tlm2ams.r( r_rtl );

  //RTL mult module
  //IN
  mult.clk(clock);
  mult.reset(reset_signal);
  mult.numbers_areready(numbers_are_ready);
  mult.number1_port(number1);
  mult.number2_port(number2);
  //OUT
  mult.result_port(result);
  mult.result_isready(result_is_ready);
  mult.r_port( r_rtl );
  
  //RTL SIDE
  //in
  rtl2tdf.r_rtl( r_rtl ); //result from RTL in bit
  //out
  rtl2tdf.y_rtl( y_rtl ); //goes to TLM

  //TDF SIDE
  //out
  rtl2tdf.r_tdf( r ); //result from RTL in bit
  //in
  rtl2tdf.y_tdf( y );  // y from plant
  
  //ERROR MODULE
  //in
  er.r( r );
  er.y( y ); // y from plant
  //out
  er.e ( e );
  
  //CONTROLLER MODULE
  ctrl.e( e );
  ctrl.k( k );
  
  //PLANT MODULE
  plt.k( k );
  plt.y( y );
  
  testbench.initiator_socket1( tlm2rtl.target_socket );
  testbench.initiator_socket2( tlm2ams.target_socket );
}

System::~System()
{
}
