#include "AlgebraDebug.h"

template<>
InputParameters validParams<AlgebraDebug>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("v","Other variable to mirror");
  return params;
}

// Phase interpolation func
AlgebraDebug::AlgebraDebug(const InputParameters & parameters) :
    Kernel(parameters),
    _v(coupledValue("v")),
    _v_var(coupled("v"))
{
}

Real
AlgebraDebug::computeQpResidual()
{
  return _test[_i][_qp] * (_u[_qp] - _v[_qp]);
}

Real
AlgebraDebug::computeQpJacobian()
{
  return _test[_i][_qp] * _phi[_j][_qp];
}

Real
AlgebraDebug::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _v_var)
    return -_test[_i][_qp] * _phi[_j][_qp];

  return 0.0;
}
