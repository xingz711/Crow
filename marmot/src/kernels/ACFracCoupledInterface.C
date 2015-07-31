/*
Formulation: Miehe et. al., Int. J. Num. Methods Engg., 2010, 83. 1273-1311
Equation 63-2
Sets up residual for auxiliary variable
Strain energy contribution moved to equation 63-3
Use with ACFracIntVar
*/
#include "ACFracCoupledInterface.h"

template<>
InputParameters validParams<ACFracCoupledInterface>()
{
  InputParameters params = validParams<KernelGrad>();

  params.addRequiredCoupledVar("c", "Order parameter");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel");
  return params;
}

ACFracCoupledInterface::ACFracCoupledInterface(const InputParameters & parameters):
    KernelGrad(parameters),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _c_var(coupled("c")),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _L(getMaterialProperty<Real>("mob_name"))
{
}

RealGradient
ACFracCoupledInterface::precomputeQpResidual()
{
  return _kappa[_qp] * _L[_qp] * _grad_c[_qp];
}

RealGradient
ACFracCoupledInterface::precomputeQpJacobian()
{
  return 0.0;
}

Real
ACFracCoupledInterface::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
    return _kappa[_qp] * _L[_qp] * (_grad_test[_i][_qp] * _grad_phi[_j][_qp]);
  else
    return 0.0;
}
