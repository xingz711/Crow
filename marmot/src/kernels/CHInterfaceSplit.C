#include "CHInterfaceSplit.h"

template<>
InputParameters validParams<CHInterfaceSplit>()
{
  InputParameters params = validParams<KernelGrad>();
  params.addParam<MaterialPropertyName>("mob_name", "M", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa", "The kappa used with the kernel");
  params.addRequiredCoupledVar("w", "Variable representing the laplacian of c");
  return params;
}

CHInterfaceSplit::CHInterfaceSplit(const InputParameters & parameters) :
    KernelGrad(parameters),
    _var_w(coupled("w")),
    _grad_w(coupledGradient("w")),
    _M(getMaterialProperty<Real>("mob_name")),
    _kappa(getMaterialProperty<Real>("kappa_name"))
{
}

RealGradient
CHInterfaceSplit::precomputeQpResidual()
{
  return -_kappa[_qp] * _M[_qp] * _grad_w[_qp];
}

RealGradient
CHInterfaceSplit::precomputeQpJacobian()
{
  return 0.0;
}

Real
CHInterfaceSplit::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _var_w)
    return -_kappa[_qp] * _M[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];

  return 0.0;
}
