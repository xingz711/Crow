#include "OrderParamInterface.h"

template<>
InputParameters validParams<OrderParamInterface>()
{
  InputParameters params = validParams<KernelGrad>();
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel");
  params.addRequiredCoupledVar("eta2","Order parameter 2");
  params.addRequiredCoupledVar("eta3","Order parameter 3");
  return params;
}

OrderParamInterface::OrderParamInterface(const InputParameters & parameters) :
    KernelGrad(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _L(getMaterialProperty<Real>("mob_name")),
    _eta2(coupledValue("eta2")),
    _grad_eta2(coupledGradient("eta2")),
    _eta3(coupledValue("eta3")),
    _grad_eta3(coupledGradient("eta3")),
    _eta2_var(coupled("eta2")),
    _eta3_var(coupled("eta3"))
{
}

RealGradient
OrderParamInterface::precomputeQpResidual()
{
  RealGradient returnGrad = 0.0;
  if (std::abs(_grad_u[_qp].size()) < 1.0e-07)
    return returnGrad;
  return _kappa[_qp] / 2 * _L[_qp] * _grad_u[_qp] * (_grad_eta2[_qp].size() / _grad_u[_qp].size() + _grad_eta3[_qp].size() / _grad_u[_qp].size());

}

RealGradient
OrderParamInterface::precomputeQpJacobian()
{
  // RealGradient returnGrad = 0.0;
  // if (std::abs(_grad_u[_qp].size()) < 1.0e-07)
    // return returnGrad;
  return 0.0;
}

RealGradient
OrderParamInterface::precomputeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _eta2_var)
    return _grad_phi[_j][_qp].size() * _grad_u[_qp] / _grad_u[_qp].size()
      * _eta2[_qp] / std::abs(_eta2[_qp]);

  else if (jvar == _eta3_var)
    return _grad_phi[_j][_qp].size() * _grad_u[_qp] / _grad_u[_qp].size()
      * _eta2[_qp] / std::abs(_eta3[_qp]);
    return 0.0;
}
