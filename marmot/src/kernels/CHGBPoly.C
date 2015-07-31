#include "CHGBPoly.h"

template<>
InputParameters validParams<CHGBPoly>()
{
  InputParameters params = validParams<CHBulk>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredParam<Real>("en_ratio", "ratio of surface energy to GB energy");
  return params;
}

CHGBPoly::CHGBPoly(const InputParameters & parameters) :
    CHBulk(parameters),
    _mu(getMaterialPropertyByName<Real>("mu")),
    _en_ratio(getParam<Real>("en_ratio"))
{
  unsigned int n = coupledComponents("v");
  _vals.resize(n);
  _grad_vals.resize(n);

  for (unsigned int i = 0; i < n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

RealGradient
CHGBPoly::computeGradDFDCons(PFFunctionType type)
{
  const Real gamma = 1.5;

  // Calculate sums of grain order parameters and their gradients
  Real grnsum = 0.0;
  RealGradient grad_grnsum = 0.0;

  for (unsigned int i=0; i<_vals.size(); ++i)
  {
    grnsum += (*_vals[i])[_qp] * (*_vals[i])[_qp];
    grad_grnsum += 2.0*(*_vals[i])[_qp] * (*_grad_vals[i])[_qp];
  }

  Real dfdc = 2.0 * _en_ratio * gamma * _mu[_qp] * _u[_qp];
  Real d2fdc2 = 2.0 * _en_ratio * gamma * _mu[_qp];

  switch (type)
  {
    case Residual:
      return _grad_u[_qp] * d2fdc2 * grnsum + dfdc * grad_grnsum;

    case Jacobian:
      return _grad_phi[_j][_qp] * d2fdc2 * grnsum + d2fdc2 * _phi[_j][_qp] * grad_grnsum; // return Jacobian value
  }

  mooseError("Internal error");
}
