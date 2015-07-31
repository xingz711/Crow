#include "ConcentrationAux.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<ConcentrationAux>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

ConcentrationAux::ConcentrationAux(const InputParameters & parameters) :
    AuxKernel(parameters)
{
}

Real
ConcentrationAux::computeValue()
{
  return 2.0 * MooseRandom::rand() - 1.0;
  // return 1.0;
}
