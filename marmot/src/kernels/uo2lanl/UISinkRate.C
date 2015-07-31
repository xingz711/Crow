#include "UISinkRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<UISinkRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MUI_name", "MUI", "UI mobility");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");

  return params;
}

UISinkRate::UISinkRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _dgHVaUI(getMaterialProperty<Real>("dgHVaUI")),
    _dgHVa(getMaterialProperty<Real>("dgHVa")),
    _dgSVa(getMaterialProperty<Real>("dgSVa")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cvv(getMaterialProperty<Real>("kappa_cVV")),
    _kappa_cui(getMaterialProperty<Real>("kappa_cUI")),
    _kappa_cu(getMaterialProperty<Real>("kappa_cU")),
    _kI2(getMaterialProperty<Real>("ki2"))
{
}

Real
UISinkRate::computeQpResidual()
{

  //Sink term, specifically for UI in the Xe, Va, XeV, VaVa, UI and Va_I system

  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R21 = 0.0;

  // Concentration terms
  Real DF = (_dgHVaUI[_qp] - (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp]) + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             - 1 * _kT[_qp] * poly4Log(1 - _u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp]));

  R1 = _test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * _kI2[_qp] * _Zg[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * _kI2[_qp] * _Zg[_qp] * DF2);

  return R1 + R21;

}

Real
UISinkRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  // Concentration terms
  Real DF = (_dgHVaUI[_qp] - (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp]) + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             - 1 * _kT[_qp] * poly4Log(1 - _u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp]));

  return _test[_i][_qp] * (_phi[_j][_qp] * (_MUI[_qp]) * _kI2[_qp] * _Zg[_qp] * DF)
         + _test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * _kI2[_qp] * _Zg[_qp] * (_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]) - 1 * _kT[_qp] * poly4Log(1 - _u[_qp],tol,1) * (-1 * _phi[_j][_qp]))
         + _grad_test[_i][_qp] * (_phi[_j][_qp] * (_MUI[_qp]) * _kI2[_qp] * _Zg[_qp] * DF2)
         + _grad_test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * _Zg[_qp] * _kappa_cui[_qp] * (_grad_phi[_j][_qp]) - 1 * _kappa_cui[_qp] * (-1 * _grad_phi[_j][_qp]));
}
