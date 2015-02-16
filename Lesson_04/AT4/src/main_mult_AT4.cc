#include "mult_AT4.hh"
#include "mult_AT4_testbench.hh"

class mult_top : public sc_module
{

private:

  mult_AT4  m_target;

  mult_AT4_testbench m_initiator;


public:

  mult_top(sc_module_name name)
    : sc_module(name)
    , m_target("target")
    , m_initiator("initiator")
  {
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int main(int argc, char* argv[])
{

  mult_top top("top");

  sc_start();

  return 0;

}
