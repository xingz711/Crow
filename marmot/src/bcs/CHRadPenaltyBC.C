#include "CHRadPenaltyBC.h"

template<>
InputParameters validParams<CHRadPenaltyBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.set<Real>("M") = 1.0;
  return params;
}

CHRadPenaltyBC::CHRadPenaltyBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _M(getParam<Real>("M"))
{
}

Real
CHRadPenaltyBC::computeQpResidual()
{
  const Real penalty = 1e10;
  return penalty * (_grad_u[_qp] * _normals[_qp]) * (_grad_test[_i][_qp] * _normals[_qp]);
}

Real
CHRadPenaltyBC::computeQpJacobian()
{
  if (_i == _j)
  {
    const Real penalty = 1e10;
    return penalty * _grad_phi[_j][_qp] * _normals[_qp] * (_grad_test[_i][_qp] * _normals[_qp]);
  }
  else
    return 0.0;
}
