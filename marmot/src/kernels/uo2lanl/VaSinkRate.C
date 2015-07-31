#include "VaSinkRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<VaSinkRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MVa_name", "MVa","Va mobility");
  params.addParam<std::string>("LogC_name", "LogC","For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol","For PolyLog");
  return params;
}

VaSinkRate::VaSinkRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MVa_name(getParam<std::string>("MVa_name")),
    _MVa(getMaterialProperty<Real>(_MVa_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
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
    _kV2(getMaterialProperty<Real>("kv2"))
{
}

Real
VaSinkRate::computeQpResidual()
{


  //Sink term, specifically for Va in the Xe, Va, XeV, VaVa, UI and Va_I system


  Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R21 = 0.0;

  // Concentration terms
  Real DF = (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp] + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             -1 * _kT[_qp] * poly4Log(_CU[_qp],tol,0));

  RealGradient DF2 = (_kappa_cv[_qp] * _grad_u[_qp] - 1 * _kappa_cu[_qp] * _grad_CU[_qp]);

  R1 = _test[_i][_qp] * (_u[_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_u[_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * DF2);

  return R1+R21;

}

Real
VaSinkRate::computeQpJacobian()
{

  Real tol = _LogTol[_qp];

  // Concentration terms
  Real DF = (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp] + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             - 1 * _kT[_qp] * poly4Log(_CU[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cv[_qp] * _grad_u[_qp] - 1 * _kappa_cu[_qp] * _grad_CU[_qp]);

  return _test[_i][_qp] * (_phi[_j][_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * DF)
         + _test[_i][_qp] * (_u[_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * (_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]) - 1 * _kT[_qp] * poly4Log(_CU[_qp],tol,1) * (-1 * _phi[_j][_qp]))
         + _grad_test[_i][_qp] * (_phi[_j][_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * DF2)
         + _grad_test[_i][_qp] * (_u[_qp] * (_MVa[_qp]) * _kV2[_qp] * _Zg[_qp] * _kappa_cv[_qp] * (_grad_phi[_j][_qp]) - 1 * _kappa_cu[_qp] * (-1 * _grad_phi[_j][_qp]));

}
