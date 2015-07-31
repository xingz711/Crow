#include "XeVa2_XeVa2BRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<XeVa2_XeVa2BRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa",  "Va mobility");
  params.addParam<std::string>("mob_name2", "MCgVa",  "XeVa diffusion");
  params.addRequiredCoupledVar("c6", "XeVaVa concentration");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");

  return params;
}

XeVa2_XeVa2BRate::XeVa2_XeVa2BRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa(getMaterialProperty<Real>(_mob_name)),
    _mob_name2(getParam<std::string>("mob_name2")),
    _MCgVa(getMaterialProperty<Real>(_mob_name2)),
    _c6(coupledValue("c6")),
    _grad_c6(coupledGradient("c6")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVa2B(getMaterialProperty<Real>("EBgXeVa2B")),
    _SBXeVa2B(getMaterialProperty<Real>("SBgXeVa2B")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgvv(getMaterialProperty<Real>("kappa_cGVV")),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV")),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV"))
{
}

Real
XeVa2_XeVa2BRate::computeQpResidual()
{

  //XeVa+Va <-> XeVaVa

  const Real tol = _LogTol[_qp];
  const Real S=0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms
  Real DF = (_EBXeVa2B[_qp] - _kT[_qp] * (_SBXeVa2B[_qp]) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c2[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgvv[_qp] * _grad_u[_qp] - _kappa_cgv[_qp] * _grad_c3[_qp]
                      - _kappa_cv[_qp] * _grad_c2[_qp]);

  R1 = _test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF);

  R2 = _test[_i][_qp] * (-_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF2);

  R22 = _grad_test[_i][_qp] * (-_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF2);

  // Check sign to avoid negative concentrations
  if (_c3[_qp] >= 0 && _c2[_qp] >= 0)
    return R1 + R21;
  else if (_c3[_qp] < 0.0 && _c2[_qp] < 0.0)
    return R1 + R21;
  else if (_c3[_qp] < 0 || _c2[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
XeVa2_XeVa2BRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;
  Real R1a=0.0;
  Real R21a=0.0;
  Real S=0.385;

  // Concentration terms
  Real DF = (_EBXeVa2B[_qp] - _kT[_qp] * (_SBXeVa2B[_qp]) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c2[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgvv[_qp] * _grad_u[_qp] - _kappa_cgv[_qp] * _grad_c3[_qp]
                      - _kappa_cv[_qp] * _grad_c2[_qp]);

  R1a = _test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF);

  R21a = _grad_test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp] * DF2);

  R1 = _test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp]
                         * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R2 = _test[_i][_qp] * (-_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S) * _Zg[_qp]
                         * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1)*_phi[_j][_qp]));

  R21 = _grad_test[_i][_qp] * (_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S)
                               * _Zg[_qp]) * (_kappa_cgvv[_qp] * _grad_phi[_j][_qp]);

  R22 = _grad_test[_i][_qp] * (-_c3[_qp] * _c2[_qp] * (_MVa[_qp]/S/S + _MCgVa[_qp]/S/S)
                               * _Zg[_qp]) * (_kappa_cgvv[_qp] * _grad_phi[_j][_qp]);


  // Check sign to avoid negative concentrations
  if (_c3[_qp] >= 0 && _c2[_qp] >= 0)
    return R1 + R21;
  else if (_c3[_qp] < 0.0 && _c2[_qp] < 0.0)
    return R1 + R21;
  else if (_c3[_qp] < 0 || _c2[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}
