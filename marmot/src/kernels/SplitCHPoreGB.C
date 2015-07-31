#include "SplitCHPoreGB.h"

template<>
InputParameters validParams<SplitCHPoreGB>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredParam<Real>("en_ratio", "ratio of surface energy to GB energy");
  return params;
}

SplitCHPoreGB::SplitCHPoreGB(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _en_ratio(getParam<Real>("en_ratio")),
    _mu(getMaterialProperty<Real>("mu")),
    _gamma(getMaterialProperty<Real>("gamma_asymm")),
    _ncrys(coupledComponents("v"))
{
  _vals.resize(_ncrys);

  for (unsigned int i = 0; i < _ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
}

Real
SplitCHPoreGB::computeDFDC(PFFunctionType type)
{
  // Calculate sums of grain order parameters and their gradients
  Real grnsum = 0.0;

  for (unsigned int i = 0; i < _ncrys; ++i)
    grnsum += (*_vals[i])[_qp] * (*_vals[i])[_qp];

  Real c = _u[_qp];

  switch (type)
  {
    case Residual:
      return _mu[_qp] * (c*c*c - c + _en_ratio * _gamma[_qp] * grnsum * (2.0 * c));

    case Jacobian:
      return _mu[_qp] * (3.0 * c*c - 1.0 + _en_ratio * _gamma[_qp] * grnsum * 2.0) * _phi[_j][_qp];
  }

  mooseError("Invalid type passed in");
}
