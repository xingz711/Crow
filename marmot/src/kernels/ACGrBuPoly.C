#include "ACGrBuPoly.h"

#include "Material.h"

template<>
InputParameters validParams<ACGrBuPoly>()
{
  InputParameters params = validParams<ACBulk>();
  params.addRequiredCoupledVar("cg", "gas concentration");
  params.addRequiredCoupledVar("v", "Array of coupled variables");
  params.addParam<MaterialPropertyName>("Ao_name", "Ao", "Coefficient that scales the energy density");
  params.addParam<MaterialPropertyName>("BetaS_name", "BetaS", "Free energy parameter that scales the free surface energy");
  params.addParam<MaterialPropertyName>("BetaGB_name", "BetaGB", "Free energy parameter that scales the grain boundary energy");
  return params;
}

ACGrBuPoly::ACGrBuPoly(const InputParameters & parameters):
    ACBulk(parameters),
    _cg(coupledValue("cg")),
    _cg_var(coupled("cg")),
    _Ao(getMaterialProperty<Real>("Ao_name")),
    _BetaS(getMaterialProperty<Real>("BetaS_name")),
    _BetaGB(getMaterialProperty<Real>("BetaGB_name"))
{
  _ncrys = coupledComponents("v");
  _vals.resize(_ncrys);
  _vals_var.resize(_ncrys);

  for (unsigned int i=0; i<_ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v",i);
  }
}

Real
ACGrBuPoly::computeDFDOP(PFFunctionType type)
{
  Real SumEtaj = 0.0;
  for (unsigned int i = 0; i < _vals.size(); ++i)
    SumEtaj += (*_vals[i])[_qp]*(*_vals[i])[_qp];

  switch (type)
  {
    case Residual:
      return   _Ao[_qp]*(_u[_qp]*_u[_qp]*_u[_qp] - _u[_qp]
             + 2.0*_u[_qp]*(_BetaGB[_qp]*SumEtaj
             + _BetaS[_qp]*_cg[_qp]*_cg[_qp]));

    case Jacobian:
      return   _phi[_j][_qp]*_Ao[_qp]*(3.0*_u[_qp]*_u[_qp] - 1.0
             + 2.0*(_BetaGB[_qp]*SumEtaj
             + _BetaS[_qp]*_cg[_qp]*_cg[_qp]));
  }

  mooseError("Invalid type passed in");
}

Real
ACGrBuPoly::computeQpOffDiagJacobian(unsigned int jvar)
{
  for (unsigned int i = 0; i < _ncrys; ++i)
    if (jvar == _vals_var[i])
    {
      // Derivative of SumEtaj
      Real dSumEtaj = 2.0 * (*_vals[i])[_qp] * _phi[_j][_qp];
      Real dDFDOP = _Ao[_qp] * 2.0 * _BetaGB[_qp] * _u[_qp] * dSumEtaj;
      return _L[_qp] * _test[_i][_qp] * dDFDOP;
    }

  if (jvar == _cg_var)
  {
    Real dDFDOP =   _Ao[_qp] * 2.0 * _BetaS[_qp]
                  * _u[_qp] * 2.0 * _cg[_qp] * _phi[_j][_qp];
    return _L[_qp] * _test[_i][_qp] * dDFDOP;
  }

  return 0.0;
}
