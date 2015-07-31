#include "Va2UIRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<Va2UIRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MVa2_name", "MVa2", "VaVa mobility");
  params.addParam<std::string>("MUI_name", "MUI", "UI mobility");
  params.addRequiredCoupledVar("c5", "VaVa concentration");
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");
  return params;
}

Va2UIRate::Va2UIRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MVa2_name(getParam<std::string>("MVa2_name")),
    _MVa2(getMaterialProperty<Real>(_MVa2_name)),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _c5(coupledValue("c5")),
    _grad_c5(coupledGradient("c5")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _dgHVaUI(getMaterialProperty<Real>("dgHVaUI")),
    _dgHVa(getMaterialProperty<Real>("dgHVa")),
    _dgSVa(getMaterialProperty<Real>("dgSVa")),
    _EBVaVa(getMaterialProperty<Real>("EBgVaVa")),
    _SBVaVa(getMaterialProperty<Real>("SBgVaVa")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cvv(getMaterialProperty<Real>("kappa_cVV")),
    _kappa_cui(getMaterialProperty<Real>("kappa_cUI")),
    _kappa_cu(getMaterialProperty<Real>("kappa_cU"))
{
}

Real
Va2UIRate::computeQpResidual()
{

  //Reaction UI+VaVa <-> Va+U+Va_I

  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms
  Real DF = _dgHVaUI[_qp] + _EBVaVa[_qp]
            - _kT[_qp] * (_SBVaVa[_qp])
            + _kT[_qp] * MathUtils::poly4Log(_c5[_qp], tol, 0) - 1.0 * _kT[_qp] * MathUtils::poly4Log(1.0 - _c5[_qp], tol, 0)
            + _kT[_qp] * MathUtils::poly4Log(_u[_qp], tol, 0) -2.0 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0)
            - (_kT[_qp] * MathUtils::poly4Log(_c2[_qp], tol, 0) - 1.0 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_c5[_qp]
                      - 1 * _kappa_cui[_qp] * (-_grad_c5[_qp]) + _kappa_cvv[_qp] * _grad_u[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cv[_qp] * _grad_c2[_qp] - 1 * _kappa_cu[_qp] * _grad_CU[_qp]));

  R1 = _test[_i][_qp] * (_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R2 = _test[_i][_qp] * (-_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  R22 = _grad_test[_i][_qp] * (-_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF2);

  if (_u[_qp] >= 0 && _c5[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c5[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c5[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}

Real
Va2UIRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real S = 0.385;

  //Concentration terms
  Real DF = _dgHVaUI[_qp] + _EBVaVa[_qp]
            - _kT[_qp] * (_SBVaVa[_qp])
            + _kT[_qp] * MathUtils::poly4Log(_c5[_qp], tol, 0) - 1.0 * _kT[_qp] * MathUtils::poly4Log(1.0 - _c5[_qp], tol, 0)
            + _kT[_qp] * MathUtils::poly4Log(_u[_qp], tol, 0) -2.0 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0)
            - (_kT[_qp] * MathUtils::poly4Log(_c2[_qp], tol, 0) - 1.0 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0));

  //Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_c5[_qp] - 1 * _kappa_cui[_qp]
                      * (-_grad_c5[_qp]) + _kappa_cvv[_qp] * _grad_u[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]
                      - (_kappa_cv[_qp] * _grad_c2[_qp] - 1 * _kappa_cu[_qp] * _grad_CU[_qp]));

  Real R1 = _test[_i][_qp] * (_phi[_j][_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF)
            + _test[_i][_qp] * (_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]) - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,1)
                                * (-2 * _phi[_j][_qp]) - (-1 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R2 = -_test[_i][_qp] * (_phi[_j][_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF)
            - _test[_i][_qp] * (_u[_qp] * _c5[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                                - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,1) * (-2*_phi[_j][_qp])
                                - (-1 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,1) * (-_phi[_j][_qp])));

  Real R21 = _grad_test[_i][_qp] * (_c5[_qp] * _phi[_j][_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp]*DF2)
             + _grad_test[_i][_qp] * (_c5[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp]
                                      * (_kappa_cvv[_qp] * (_grad_phi[_j][_qp]) - 2 * _kappa_cu[_qp] * (-2 * _grad_phi[_j][_qp])
                                         - (-1 * _kappa_cu[_qp] * (-2 * _grad_phi[_j][_qp]))));

  Real R22 =  -_grad_test[_i][_qp] * (_c5[_qp] * _phi[_j][_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S) * _Zg[_qp] * DF2)
              - _grad_test[_i][_qp] * (_c5[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa2[_qp]/S/S)
                                       * _Zg[_qp] * (_kappa_cvv[_qp] * (_grad_phi[_j][_qp]) - 2.0 * _kappa_cu[_qp] * (-2.0 * _grad_phi[_j][_qp])
                                           - (-1 * _kappa_cu[_qp] * (-2 * _grad_phi[_j][_qp]))));

  if (_u[_qp] >= 0 && _c5[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _c5[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _c5[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
