#include "system.hh"


System::System( sc_core::sc_module_name ) :
  testbench("testbench"),
  tlm2rtl("tlm2rtl"),
  rtl2tdf("rtl2tdf"),
  er("er"),
  ctrl("ctrl"),
  plt("plt")
{

  //TLM SOCKET
  //RTL SIDE
  tlm2rtl.r( r_rtl );
  tlm2rtl.y( y_rtl );
  
  //RTL SIDE
  //in
  rtl2tdf.r_rtl( r_rtl );
  //out
  rtl2tdf.y_rtl( y_rtl );
  //TDF SIDE
  //out
  rtl2tdf.r_tdf( r );
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
  
  testbench.initiator_socket( tlm2rtl.target_socket );
}

System::~System()
{
}
