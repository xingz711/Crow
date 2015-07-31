#include "CHGrBu.h"

template<>
InputParameters validParams<CHGrBu>()
{
  InputParameters params = validParams<SplitCHCRes>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<MaterialPropertyName>("Ao_name", "Ao", "Coefficient that scales the energy density");
  params.addParam<MaterialPropertyName>("BetaS_name", "BetaS", "Free energy parameter that scales the free energy");
  return params;
}

CHGrBu::CHGrBu(const InputParameters & parameters) :
    SplitCHCRes(parameters),
    _Ao(getMaterialProperty<Real>("Ao_name")),
    _BetaS(getMaterialProperty<Real>("BetaS_name")),
    _nvals(coupledComponents("v"))
{
  _vals.resize(_nvals);
  for (unsigned int i = 0; i < _nvals; ++i)
    _vals[i] = &coupledValue("v", i);
}

Real
CHGrBu::computeDFDC(PFFunctionType type)
{
  Real c = _u[_qp];

  Real SumEta = 0.0;
  for (unsigned int i = 0; i < _nvals; ++i)
    SumEta += (*_vals[i])[_qp] * (*_vals[i])[_qp];

  switch (type)
  {
    case Residual:
      return _Ao[_qp] * (c*c*c - c + 2.0*c*_BetaS[_qp]*SumEta);

    case Jacobian:
      return _Ao[_qp] * _phi[_j][_qp] * (3.0*c*c - 1.0 + 2.0*_BetaS[_qp]*SumEta); // return Off-diag Jacobian value
  }

  mooseError("Invalid type passed in");
}
