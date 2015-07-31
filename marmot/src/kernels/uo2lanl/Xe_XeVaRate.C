#include "Xe_XeVaRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<Xe_XeVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MVa",  "Va mobility");
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");

  return params;
}

Xe_XeVaRate::Xe_XeVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MVa(getMaterialProperty<Real>(_mob_name)),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVa(getMaterialProperty<Real>("EBgXeVa")),
    _SBXeVa(getMaterialProperty<Real>("SBgXeVa")),
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
Xe_XeVaRate::computeQpResidual()
{

  //Reaction: Xe+Va <-> XeVa

  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  // Concentration terms
  Real DF = (_EBXeVa[_qp] - _kT[_qp] * _SBXeVa[_qp] + _kT[_qp]*poly4Log(_c3[_qp],tol,0)
             - _kT[_qp]*poly4Log(_u[_qp],tol,0) - _kT[_qp]*poly4Log(_c2[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c3[_qp] - _kappa_cg[_qp] * _grad_u[_qp]
                      - _kappa_cv[_qp] * _grad_c2[_qp]);

  Real R1 = -_test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real R2 = -_test[_i][_qp] * (-_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real R21 = -_grad_test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF2);

  Real R22 = -_grad_test[_i][_qp] * (-_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF2);

  if (_u[_qp] >= 0 && _c2[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c2[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c2[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
Xe_XeVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  // Concentration terms

  // Gradient terms
  RealGradient DF2 = (_kappa_cgv[_qp] * _grad_c3[_qp] - _kappa_cg[_qp] * _grad_u[_qp]
                      - _kappa_cv[_qp]*_grad_c2[_qp]);

  Real R1 = -_test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp]
                               * (_EBXeVa[_qp] - _kT[_qp] * _SBXeVa[_qp] + _kT[_qp] * poly4Log(_c3[_qp],tol,0)
                                  - _kT[_qp]*poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(_c2[_qp],tol,0))
                               + _u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * (-_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  Real R2 = _test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp]
                              * (_EBXeVa[_qp] - _kT[_qp] * _SBXeVa[_qp] + _kT[_qp] * poly4Log(_c3[_qp],tol,0)
                                 - _kT[_qp]*poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(_c2[_qp],tol,0))
                              + _u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * (-_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  Real R21 = -_grad_test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF2)
             - _grad_test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * (_kappa_cg[_qp] * (-_grad_phi[_j][_qp])));

  Real R22 = _grad_test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MVa[_qp]/S/S) * _Zg[_qp] * (_kappa_cg[_qp] * (-_grad_phi[_j][_qp])));

  if (_u[_qp] >= 0 && _c2[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c2[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c2[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
