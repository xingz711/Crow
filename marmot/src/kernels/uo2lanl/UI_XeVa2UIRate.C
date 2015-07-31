#include "UI_XeVa2UIRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UI_XeVa2UIRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MXeVa2_name", "MXeVa2",  "XeVaVa mobility");
  params.addParam<std::string>("MUI_name", "MUI",  "UI mobility");
  params.addRequiredCoupledVar("c3", "VaVa concentration");
  params.addRequiredCoupledVar("c6", "XeVaVa concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");
  return params;
}

UI_XeVa2UIRate::UI_XeVa2UIRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MXeVa2_name(getParam<std::string>("MXeVa2_name")),
    _MXeVa2(getMaterialProperty<Real>(_MXeVa2_name)),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _c6(coupledValue("c6")),
    _grad_c6(coupledGradient("c6")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _dgHXeVa2UI(getMaterialProperty<Real>("dgHXeVa2UI")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV")),
    _kappa_cgvv(getMaterialProperty<Real>("kappa_c")),
    _kappa_cui(getMaterialProperty<Real>("kappa_cUI")),
    _kappa_cu(getMaterialProperty<Real>("kappa_cU"))
{
}

Real
UI_XeVa2UIRate::computeQpResidual()
{

  //Reaction: XeVaVa+UI <-> XeVa+Va_I

  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  // Concentration terms
  Real DF = (_dgHXeVa2UI[_qp] + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,0) + _kT[_qp] * MathUtils::poly4Log(_c6[_qp],tol,0)
             - 3 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0) - (_kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0)
                 - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0)));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp])
                      + _kappa_cgvv[_qp] * _grad_c6[_qp] - 3 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cgv[_qp] * _grad_c3[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]));

  Real  R1 = _test[_i][_qp] * (_c6[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp] * DF);

  Real  R2 = _test[_i][_qp] * (-_c6[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp] * DF);

  Real  R21 = _grad_test[_i][_qp] * (_c6[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  Real  R22 = _grad_test[_i][_qp] * (-_c6[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  if (_c6[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c6[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c6[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
UI_XeVa2UIRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  // Concentration terms
  Real DF = (_dgHXeVa2UI[_qp] + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0) - _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,0)
             + _kT[_qp] * MathUtils::poly4Log(_c6[_qp],tol,0) - 3 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0)
             - (_kT[_qp] * MathUtils::poly4Log(_c3[_qp],tol,0) - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0)));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp])
                      + _kappa_cgvv[_qp] * _grad_c6[_qp] - 3 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cgv[_qp] * _grad_c3[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]));

  Real R1 = _test[_i][_qp] * (_phi[_j][_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]) * DF
            + _test[_i][_qp] * (_u[_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]
                                   - 1 * _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R2= -_test[_i][_qp] * (_phi[_j][_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]) * DF
           + _test[_i][_qp] * (_u[_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]
                               * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]
                                  - 1 * _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R21 = _grad_test[_i][_qp] * (_phi[_j][_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp] * DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]
                                      * (_kappa_cui[_qp] * (_grad_phi[_j][_qp]) - _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  Real R22 = -_grad_test[_i][_qp] * (_phi[_j][_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]*DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _c6[_qp] * (_MUI[_qp]/S/S + _MXeVa2[_qp]/S/S) * _Zg[_qp]
                                      * (_kappa_cui[_qp] * (_grad_phi[_j][_qp]) - _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  if (_c6[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c6[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c6[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
