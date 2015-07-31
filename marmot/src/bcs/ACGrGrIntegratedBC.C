#include "ACGrGrIntegratedBC.h"

template<>
InputParameters validParams<ACGrGrIntegratedBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel");
  return params;
}

ACGrGrIntegratedBC::ACGrGrIntegratedBC(const InputParameters & parameters) :
    IntegratedBC(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa(getMaterialProperty<Real>("kappa_name"))
{
}

Real
ACGrGrIntegratedBC::computeQpResidual()
{
  return _L[_qp] * _kappa[_qp] * _grad_u[_qp] * _test[_i][_qp] * _normals[_qp];
}

Real
ACGrGrIntegratedBC::computeQpJacobian()
{
 return _L[_qp] * _kappa[_qp] * _grad_phi[_j][_qp] * _test[_i][_qp] * _normals[_qp];
}
