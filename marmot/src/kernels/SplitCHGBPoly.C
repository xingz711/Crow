#include "SplitCHGBPoly.h"

template<>
InputParameters validParams<SplitCHGBPoly>()
{
  InputParameters params = validParams<SplitCHBase>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredParam<Real>("en_ratio", "ratio of surface energy to GB energy");
  return params;
}

SplitCHGBPoly::SplitCHGBPoly(const InputParameters & parameters) :
    SplitCHBase(parameters),
    _en_ratio(getParam<Real>("en_ratio")),
    _mu(getMaterialPropertyByName<Real>("mu")),
    _ncrys(coupledComponents("v"))
{
  _vals.resize(_ncrys);
  for (unsigned int i = 0; i < _ncrys; ++i)
    _vals[i] = &coupledValue("v", i);
}

Real
SplitCHGBPoly::computeDFDC(PFFunctionType type)
{
  const Real gamma = 1.5;

  // Calculate sums of grain order parameters and their gradients
  Real grnsum = 0.0;
  for (unsigned int i=0; i<_ncrys; ++i)
    grnsum += (*_vals[i])[_qp]*(*_vals[i])[_qp];

  switch (type)
  {
    case Residual:
      return _mu[_qp] * _en_ratio * gamma * grnsum * (2.0*_u[_qp]);

    case Jacobian:
      return _mu[_qp] *_en_ratio * gamma *grnsum * (2.0*_phi[_j][_qp]);
  }

  mooseError("Invalid type passed in");
}
