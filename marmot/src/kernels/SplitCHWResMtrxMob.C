#include "SplitCHWResMtrxMob.h"
template<>
InputParameters validParams<SplitCHWResMtrxMob>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("c","intermediate parameter--concentration");
  params.addParam<MaterialPropertyName>("mob_name","mobtemp","The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("dmob_name","mobtemp","The mobility derivative used with the kernel");
  return params;
}

SplitCHWResMtrxMob::SplitCHWResMtrxMob(const InputParameters & parameters) :
    Kernel(parameters),
    _mob(getMaterialProperty<RealTensorValue>("mob_name")),
    _dmobdc(getMaterialProperty<RealTensorValue>("dmob_name")),
    // This _c_var is needed to compute off-diagonal Jacobian.
    _c_var(coupled("c")),
    _c(coupledValue("c"))
{
}

Real
SplitCHWResMtrxMob::computeQpResidual()
{
  return  _mob[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];
}

Real
SplitCHWResMtrxMob::computeQpJacobian()
{
  return _mob[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}

Real
SplitCHWResMtrxMob::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _c_var)
    return _dmobdc[_qp] * _grad_u[_qp] * _grad_test[_i][_qp];

  return 0.0;
}
