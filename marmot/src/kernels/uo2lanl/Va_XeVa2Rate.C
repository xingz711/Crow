#include "Va_XeVa2Rate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<Va_XeVa2Rate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa2", "VaVa mobility");
  params.addParam<std::string>("mob_name2", "MCgVa", "XeVa mobility");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  params.addRequiredCoupledVar("c1", "Xe concentration");
  params.addRequiredCoupledVar("c4", "VaVa concentration");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");
  return params;
}

Va_XeVa2Rate::Va_XeVa2Rate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa2(getMaterialProperty<Real>(_mob_name)),
    _mob_name2(getParam<std::string>("mob_name2")),
    _MCgVa(getMaterialProperty<Real>(_mob_name2)),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _c4(coupledValue("c4")),
    _grad_c4(coupledGradient("c4")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVa(getMaterialProperty<Real>("EBgXeVa")),
    _SBXeVa(getMaterialProperty<Real>("SBgXeVa")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV")),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cg(getMaterialProperty<Real>("kappa_c")),
    _kappa_cvv(getMaterialProperty<Real>("kappa_cVV"))
{
}

Real
Va_XeVa2Rate::computeQpResidual()
{

  //Reaction: Xe+VaVa <-> XeVa+Va

  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms
  Real DF = (_EBXeVa[_qp] - _EBVaVa[_qp] - _kT[_qp] * (_SBXeVa[_qp] - _SBVaVa[_qp])
             + _kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_c1[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c4[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c3[_qp] + _kappa_cv[_qp] * _grad_u[_qp]
                      - _kappa_cg[_qp] * _grad_c1[_qp] - _kappa_cvv[_qp] * _grad_c4[_qp]);

  R1 = _test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R2 = _test[_i][_qp] * (-_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  R22 = _grad_test[_i][_qp] * (-_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  // Check sign to avoid negative concentrations
  if (_c1[_qp] >= 0 && _c4[_qp] >= 0)
    return R1 + R21;
  else if (_c1[_qp] < 0.0 && _c4[_qp] < 0.0)
    return R1 + R21;
  else if (_c1[_qp] < 0 || _c4[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
Va_XeVa2Rate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;
  Real R1a=0.0;
  Real R21a=0.0;

  // Concentration terms
  Real DF = (_EBXeVa[_qp] - _EBVaVa[_qp] - _kT[_qp] * (_SBXeVa[_qp] - _SBVaVa[_qp])
             + _kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_c1[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c4[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c3[_qp] + _kappa_cv[_qp] * _grad_u[_qp]
                      - _kappa_cg[_qp] * _grad_c1[_qp] - _kappa_cvv[_qp] * _grad_c4[_qp]);

  R1a = _test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R21a = _grad_test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  R1 = _test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R2 = _test[_i][_qp] * (-_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R21 = _grad_test[_i][_qp] * (_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kappa_cv[_qp] * _grad_phi[_j][_qp]));

  R22 = _grad_test[_i][_qp] * (-_c1[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kappa_cv[_qp] * _grad_phi[_j][_qp]));

  if (_c1[_qp] >= 0 && _c4[_qp] >= 0)
    return R1 + R21;
  else if (_c1[_qp] < 0.0 && _c4[_qp] < 0.0)
    return R1 + R21;
  else if (_c1[_qp] < 0 || _c4[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}
