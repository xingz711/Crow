#include "UIVaRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UIVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MVa_name","MVa","Va mobility");
  params.addParam<std::string>("MUI_name","MI","UI mobility");
  params.addRequiredCoupledVar("c2","Va concentration");
  params.addParam<std::string>("LogC_name","LogC","For PolyLog");
  params.addParam<std::string>("LogTol_name","LogTol","For PolyLog");

  return params;
}

UIVaRate::UIVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MVa_name(getParam<std::string>("MVa_name")),
    _MVa(getMaterialProperty<Real>(_MVa_name)),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _dgHVaUI(getMaterialProperty<Real>("dgHVaUI")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cui(getMaterialProperty<Real>("kappa_cUI")),
    _kappa_cu(getMaterialProperty<Real>("kappa_cU"))
{
}

Real
UIVaRate::computeQpResidual()
{

  //Reaction: UI+Va <->U+Va_I

  const Real tol = _LogTol[_qp];

  Real S=0.385;

  // Concentration terms
  Real DF =   _dgHVaUI[_qp]
              + _kT[_qp] * MathUtils::poly4Log(_u[_qp], tol, 0)
              - _kT[_qp] * MathUtils::poly4Log(1.0 - _u[_qp], tol, 0)
              + _kT[_qp] * MathUtils::poly4Log(_c2[_qp], tol, 0)
              - _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0);

  // Gradient terms
  RealGradient DF2 = (_kappa_cv[_qp] * (_grad_c2[_qp]) - 1 * _kappa_cu[_qp] * (_grad_CU[_qp])
                      + _kappa_cui[_qp] * _grad_u[_qp]) - 1 * _kappa_cui[_qp] * (-_grad_u[_qp]);

  Real R1 = _test[_i][_qp] * (_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real R2 = _test[_i][_qp] * (-_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF);

  Real R21 = _grad_test[_i][_qp] * (_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF2);

  Real R22 = _grad_test[_i][_qp] * (-_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF2);

  if (_c2[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c2[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c2[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}

Real
UIVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  // Concentration terms
  Real DF =   _dgHVaUI[_qp]
              + _kT[_qp] * MathUtils::poly4Log(_u[_qp], tol, 0)
              - _kT[_qp] * MathUtils::poly4Log(1.0 - _u[_qp],tol, 0)
              + _kT[_qp] * MathUtils::poly4Log(_c2[_qp], tol, 0)
              - _kT[_qp] * MathUtils::poly4Log(_CU[_qp], tol, 0);

  // Gradient terms
  RealGradient DF2 =   (_kappa_cv[_qp] * _grad_c2[_qp] - 1.0 * _kappa_cu[_qp] * _grad_CU[_qp]
                        + _kappa_cui[_qp] * _grad_u[_qp])
                       - 1.0 * _kappa_cui[_qp]*(-_grad_u[_qp]);

  Real S=0.385;

  Real R1 = _test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp]) * DF
            + _test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                                - _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,1) * (-_phi[_j][_qp]));

  Real R2 = -_test[_i][_qp] * (_phi[_j][_qp] * _c2[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp]) * DF
            - _test[_i][_qp] * (_u[_qp] * _c2[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp]
                                * (_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                                - _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,1) * (-_phi[_j][_qp]));

  Real R21 = _grad_test[_i][_qp] * (_c2[_qp] * _phi[_j][_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF2)
             + _grad_test[_i][_qp] * (_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S)
                                      * _Zg[_qp] * (_kappa_cui[_qp] * (_grad_phi[_j][_qp])
                                          - 1 * _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  Real R22 = -_grad_test[_i][_qp] * (_c2[_qp] * _phi[_j][_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S) * _Zg[_qp] * DF2)
             - _grad_test[_i][_qp] * (_c2[_qp] * _u[_qp] * (_MUI[_qp]/S/S + _MVa[_qp]/S/S)
                                      * _Zg[_qp] * (_kappa_cui[_qp] * (_grad_phi[_j][_qp])
                                          - 1 * _kappa_cui[_qp] * (-_grad_phi[_j][_qp])));

  if (_c2[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_c2[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_c2[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;

}
