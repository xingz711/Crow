#include "CHInterface4.h"

template<>
InputParameters validParams<CHInterface4>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<MaterialPropertyName>("kappa_name","The kappa used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("mob_name","The mobility used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("grad_mob_name","The gradient of the mobility used with the kernel");
  return params;
}

CHInterface4::CHInterface4(const InputParameters & parameters) :
    Kernel(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getMaterialProperty<Real>("mob_name")),
    _grad_M(getMaterialProperty<RealGradient>("grad_mob_name")),
    _second_u(second()),
    _second_test(secondTest()),
    _second_phi(secondPhi())
{
}

//Calculates the gradient energy term specifically for the Xe, Va, XeVa and VaVa system
Real
CHInterface4::computeQpResidual()
{
  Real value = 0.0;
  RealTensor second_c;

  second_c = _second_u[_qp];

  value = _kappa[_qp]*second_c.tr()
          * ( _M[_qp] * _u[_qp] * _second_test[_i][_qp].tr()
              + _M[_qp] * _grad_u[_qp] * _grad_test[_i][_qp]);

  return value;
}

Real
CHInterface4::computeQpJacobian()
{
  Real value = 0.0;
  RealTensor second_c = _second_u[_qp];

  value = _kappa[_qp]*_second_phi[_j][_qp].tr()
          * ( _M[_qp] * _u[_qp] * _second_test[_i][_qp].tr()
              + _M[_qp] * _grad_u[_qp] * _grad_test[_i][_qp])
          + _kappa[_qp] * second_c.tr()
          * (_M[_qp] * _phi[_j][_qp] * _second_test[_i][_qp].tr() + _M[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp]);

  return value;
}
