#include "mult_UT.hh"
#include "mult_UT_testbench.hh"

class mult_top : public sc_module
{

private:

  mult_UT  m_target;

  mult_UT_testbench m_initiator;


public:

  mult_top(sc_module_name name)
    : sc_module(name)
    , m_target("target")
    , m_initiator("initiator")
  {
    // bind the TLM ports
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int main(int argc, char* argv[])
{

  mult_top top("top");
  sc_start();

  return 0;

}
