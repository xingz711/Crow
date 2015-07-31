#include "VaAux.h"

template<>
InputParameters validParams<VaAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("coupled1", "Coupled variable");
  params.addRequiredCoupledVar("coupled2", "Coupled variable");
  return params;
}

VaAux::VaAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _coupled_val1(coupledValue("coupled1")),
    _coupled_val2(coupledValue("coupled2"))
{
}

Real
VaAux::computeValue()
{
  return 1.0 - _coupled_val1[_qp] - _coupled_val2[_qp];
}
