#include "VaVaRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<VaVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa",  "Va mobility");
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");
  return params;
}

VaVaRate::VaVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa(getMaterialProperty<Real>(_mob_name)),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cvv(getMaterialProperty<Real>("kappa_cVV")),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV"))
{
}

Real
VaVaRate::computeQpResidual()
{

  //Reaction: Va+Va <-> VaVa

  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  // Concentration terms
  Real DF = (_EBVaVa[_qp] - _kT[_qp] * _SBVaVa[_qp] + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - 2 * _kT[_qp] * MathUtils::poly4Log(_c2[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cvv[_qp] * _grad_u[_qp] - _kappa_cv[_qp] * _grad_c2[_qp] * 2);

  Real R1 = _test[_i][_qp] * (_c2[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real R21 = _grad_test[_i][_qp] * (_c2[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF2);

  return R1 + R21;
}

Real
VaVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  return   _test[_i][_qp] * (_c2[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]))
           + _grad_test[_i][_qp] * (_c2[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * _kappa_cvv[_qp] * _grad_phi[_j][_qp]);
}
