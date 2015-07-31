#include "Xe_XeVa2MRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<Xe_XeVa2MRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa2",  "Va2 mobility");
  params.addRequiredCoupledVar("c6", "XeVaVa concentration");
  params.addRequiredCoupledVar("c1", "Xe concentration");
  params.addRequiredCoupledVar("c4", "VaVa concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");

  return params;
}

Xe_XeVa2MRate::Xe_XeVa2MRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa2(getMaterialProperty<Real>(_mob_name)),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c6(coupledValue("c6")),
    _grad_c6(coupledGradient("c6")),
    _c4(coupledValue("c4")),
    _grad_c4(coupledGradient("c4")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVa2(getMaterialProperty<Real>("EBgXeVa2")),
    _SBXeVa2(getMaterialProperty<Real>("SBgXeVa2")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgvv(getMaterialProperty<Real>("kappa_cGVV")),
    _kappa_cg(getMaterialProperty<Real>("kappa_c")),
    _kappa_cvv(getMaterialProperty<Real>("kappa_cVV"))
{
}

Real
Xe_XeVa2MRate::computeQpResidual()
{

  //Reaction: Xe+VaVa <-> XeVaVa

  const Real tol = _LogTol[_qp];
  const Real S=0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms
  Real DF = (_EBXeVa2[_qp] - _kT[_qp] * (_SBXeVa2[_qp]) + _kT[_qp] * MathUtils::poly4Log(_c6[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c4[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgvv[_qp] * _grad_c6[_qp]
                      - _kappa_cg[_qp] * _grad_u[_qp] - _kappa_cvv[_qp] * _grad_c4[_qp]);

  R1 = -_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R2 = -_test[_i][_qp] * (-_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R21 = -_grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  R22 = -_grad_test[_i][_qp] * (-_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  // Check sign to avoid negative concentrations
  if (_u[_qp] >= 0 && _c4[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c4[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c4[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
Xe_XeVa2MRate::computeQpJacobian()
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
  Real DF = (_EBXeVa2[_qp] - _kT[_qp] * (_SBXeVa2[_qp]) + _kT[_qp] * MathUtils::poly4Log(_c6[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(_c4[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgvv[_qp] * _grad_c6[_qp]
                      - _kappa_cg[_qp] * _grad_u[_qp] - _kappa_cvv[_qp] * _grad_c4[_qp]);

  R1a = _test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R21a = _grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  R1 = -_test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF)
       + _test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                           * (-_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R2 = -_test[_i][_qp] * (-_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF)
       + _test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                           * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R21 = -_grad_test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2)
        + _grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                                 * (-_kappa_cg[_qp] * _grad_phi[_j][_qp]));

  R22 = -_grad_test[_i][_qp] * (-_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2)
        + _grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                                 * (+_kappa_cg[_qp] * _grad_phi[_j][_qp]));

  // Check sign to avoid negative concentrations
  if (_u[_qp] >= 0 && _c4[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c4[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c4[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}
