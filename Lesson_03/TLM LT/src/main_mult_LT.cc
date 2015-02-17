#include "mult_LT.hh"
#include "mult_LT_testbench.hh"

class mult_top : public sc_module
{

private:

  mult_LT  m_target;

  mult_LT_testbench m_initiator;


public:

  mult_top(sc_module_name name)
    : sc_module(name)
    , m_target("target")
    , m_initiator("initiator")
  {
    // bind TLM ports
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int main(int argc, char* argv[])
{

  mult_top top("top");
  sc_start();

  return 0;

}
