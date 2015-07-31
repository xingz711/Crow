#include "UIXeVaRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<UIXeVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MXeVa_name", "MXeVa", "XeVa mobility");
  params.addParam<std::string>("MUI_name", "MUI", "UI mobility");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  params.addRequiredCoupledVar("c1", "Xe concentration");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");
  return params;
}

UIXeVaRate::UIXeVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MXeVa_name(getParam<std::string>("MXeVa_name")),
    _MXeVa(getMaterialProperty<Real>(_MXeVa_name)),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _dgHXeVaUI(getMaterialProperty<Real>("dgHXeVaUI")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV")),
    _kappa_cg(getMaterialProperty<Real>("kappa_c")),
    _kappa_cui(getMaterialProperty<Real>("kappa_cUI")),
    _kappa_cu(getMaterialProperty<Real>("kappa_cU"))
{
}

Real
UIXeVaRate::computeQpResidual()
{

  //UI+XeVa<->Xe+U+Va_I

  const Real tol = _LogTol[_qp];

  Real S=0.385;

  // Concentration terms
  Real DF = (_dgHXeVaUI[_qp] + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * poly4Log(1 - _u[_qp],tol,0) + _kT[_qp] * poly4Log(_c3[_qp],tol,0)
             - 2 * _kT[_qp] * poly4Log(_CU[_qp],tol,0) - (_kT[_qp]*poly4Log(_c1[_qp],tol,0) - _kT[_qp] * poly4Log(_CU[_qp],tol,0)));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp])
                      + _kappa_cgv[_qp] * _grad_c3[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cg[_qp] * _grad_c1[_qp] - _kappa_cu[_qp] * _grad_CU[_qp]));

  Real  R1 = _test[_i][_qp] * (_c3[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real  R2 = _test[_i][_qp] * (-_c3[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real  R21 = _grad_test[_i][_qp] * (_c3[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF2);

  Real  R22 = _grad_test[_i][_qp] * (-_c3[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF2);

  if (_c3[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c3[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c3[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
UIXeVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real S=0.385;

  // Concentration terms
  Real DF = (_dgHXeVaUI[_qp] + _kT[_qp] * poly4Log(_u[_qp],tol,0) - _kT[_qp] * poly4Log(1-_u[_qp],tol,0)
             + _kT[_qp] * poly4Log(_c3[_qp],tol,0) - 2 * _kT[_qp] * poly4Log(_CU[_qp],tol,0)
             - (_kT[_qp] * poly4Log(_c1[_qp],tol,0) - _kT[_qp]*poly4Log(_CU[_qp],tol,0)));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp]*_grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp])
                      + _kappa_cgv[_qp] * _grad_c3[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cg[_qp] * _grad_c1[_qp] - _kappa_cu[_qp] * _grad_CU[_qp]));

  Real R1 = _test[_i][_qp] * (_phi[_j][_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]) * DF
            + _test[_i][_qp] * (_u[_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]
                                   - _kT[_qp]*poly4Log(1-_u[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R2= -_test[_i][_qp] * (_phi[_j][_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]) * DF
           - _test[_i][_qp] * (_u[_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]
                               * (_kT[_qp]*poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]
                                  - _kT[_qp] * poly4Log(1-_u[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R21 = _grad_test[_i][_qp] * (_phi[_j][_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]
                                      * (_kappa_cui[_qp] * _grad_phi[_j][_qp] - 1 * _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  Real R22 = -_grad_test[_i][_qp] * (_phi[_j][_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp] * DF2)
             - _grad_test[_i][_qp] * (_u[_qp] * _c3[_qp] * (_MUI[_qp]/S/S + _MXeVa[_qp]/S/S) * _Zg[_qp]
                                      * (_kappa_cui[_qp] * _grad_phi[_j][_qp] - 1 * _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  if (_c3[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c3[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c3[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
