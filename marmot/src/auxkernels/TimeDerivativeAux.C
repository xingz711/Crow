#include "TimeDerivativeAux.h"

template<>
InputParameters validParams<TimeDerivativeAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("u", "Variable that you want the time derivative of");

  return params;
}

TimeDerivativeAux::TimeDerivativeAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _u_dot(coupledDot("u"))
{
}

Real
TimeDerivativeAux::computeValue()
{
  return _u_dot[_qp];
}
