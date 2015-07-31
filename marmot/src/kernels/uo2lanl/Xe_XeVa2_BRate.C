#include "Xe_XeVa2_BRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<Xe_XeVa2_BRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa2",  "VaVa mobility");
  params.addRequiredCoupledVar("c4", "Va2 concentration");
  params.addRequiredCoupledVar("c6", "XeVa2 concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");
  return params;
}

Xe_XeVa2_BRate::Xe_XeVa2_BRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa2(getMaterialProperty<Real>(_mob_name)),
    _c4(coupledValue("c4")),
    _grad_c4(coupledGradient("c4")),
    _c6(coupledValue("c6")),
    _grad_c6(coupledGradient("c6")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVa2(getMaterialProperty<Real>("EBgXeVa2")),
    _SBXeVa2(getMaterialProperty<Real>("SBgXeVa2")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV")),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cg(getMaterialProperty<Real>("kappa_c"))
{
}

Real
Xe_XeVa2_BRate::computeQpResidual()
{

  //Xe+VaVa <-> XeVaVa

  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  // Concentration terms
  Real DF = (_EBXeVa2[_qp] - _kT[_qp] * _SBXeVa2[_qp] + _kT[_qp]*poly4Log(_c6[_qp],tol,0)
             - _kT[_qp] * poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(_c4[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c6[_qp] - _kappa_cg[_qp] * _grad_u[_qp]
                      - _kappa_cv[_qp] * _grad_c4[_qp]);

  Real R1 = -_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  Real R2 = -_test[_i][_qp] * (-_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  Real R21 = -_grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  Real R22 = -_grad_test[_i][_qp] * (-_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

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
Xe_XeVa2_BRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];
  Real S = 0.385;

  // Concentration terms

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c6[_qp] - _kappa_cg[_qp] * _grad_u[_qp]
                      - _kappa_cv[_qp] * _grad_c4[_qp]);

  Real R1 = -_test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                               * (_EBXeVa2[_qp] - _kT[_qp] * _SBXeVa2[_qp] + _kT[_qp] * poly4Log(_c6[_qp],tol,0)
                                  - _kT[_qp] * poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(_c4[_qp],tol,0))
                               + _u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (-_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  Real R2 = _test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp]
                              * (_EBXeVa2[_qp] - _kT[_qp] * _SBXeVa2[_qp] + _kT[_qp] * poly4Log(_c6[_qp],tol,0)
                                 - _kT[_qp]*poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(_c4[_qp],tol,0))
                              + _u[_qp]*_c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (-_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  Real R21 = -_grad_test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2)
             - _grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kappa_cg[_qp] * (-_grad_phi[_j][_qp])));

  Real R22 = _grad_test[_i][_qp] * (_phi[_j][_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _c4[_qp] * (_MVa2[_qp]/S/S) * _Zg[_qp] * (_kappa_cg[_qp] * (-_grad_phi[_j][_qp])));

  if (_u[_qp] >= 0 && _c4[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c4[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c4[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
