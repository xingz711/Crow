#include "CHInterface3.h"

template<>
InputParameters validParams<CHInterface3>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<MaterialPropertyName>("kappa_name", "The kappa used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("mob_name", "The mobility used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("grad_mob_name", "The gradient of the mobility used with the kernel");
  params.addRequiredCoupledVar("c1", "Xe Concentration");
  params.addRequiredCoupledVar("c2", "Va Concentration");
  params.addRequiredCoupledVar("c3", "XeVa Concentration");
  params.addRequiredCoupledVar("c4", "VaVa Concentration");
  return params;
}

CHInterface3::CHInterface3(const InputParameters & parameters) :
    Kernel(parameters),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _M(getMaterialProperty<Real>("mob_name")),
    _grad_M(getMaterialProperty<RealGradient>("grad_mob_name")),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _second_c1(coupledSecond("c1")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _second_c2(coupledSecond("c2")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _second_c3(coupledSecond("c3")),
    _c4(coupledValue("c4")),
    _grad_c4(coupledGradient("c4")),
    _second_c4(coupledSecond("c4")),
    _second_test(secondTest()),
    _second_phi(secondPhi())
{
}

Real
CHInterface3::computeQpResidual()
{
  //Calculates the gradient energy term specifically for the Xe, Va, XeVa and VaVa system

  RealTensor second_c =        -_second_c1[_qp] - _second_c2[_qp]
                               - 2.0 * _second_c3[_qp] - 2.0 * _second_c4[_qp];
  Real value =   _kappa[_qp] * second_c.tr()
                 * (   _M[_qp] * _u[_qp] * _second_test[_i][_qp].tr()
                       + _M[_qp] * _grad_u[_qp] * _grad_test[_i][_qp]
                   );

  return value;
}

Real
CHInterface3::computeQpJacobian()
{
  RealTensor second_c =        -_second_c1[_qp] - _second_c2[_qp]
                               - 2.0 * _second_c3[_qp] - 2.0 * _second_c4[_qp];

  Real value =   -2.0 * _kappa[_qp] * _second_phi[_j][_qp].tr()
                 * (   _M[_qp] * _u[_qp] * _second_test[_i][_qp].tr()
                       + _M[_qp] * _grad_u[_qp] * _grad_test[_i][_qp]
                   )
                 + _kappa[_qp] * second_c.tr()
                 * (   _M[_qp] * _phi[_j][_qp] * _second_test[_i][_qp].tr()
                       + _M[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp]
                   );

  return value;
}
