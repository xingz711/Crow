#include "XeSinkRate.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<XeSinkRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MXe_name", "MXe","Xe mobility");
  params.addParam<std::string>("LogC_name", "LogC","For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol","For PolyLog");
  return params;
}

XeSinkRate::XeSinkRate(const InputParameters & parameters) :
    Kernel(parameters),
    _MXe_name(getParam<std::string>("MXe_name")),
    _MXe(getMaterialProperty<Real>(_MXe_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _kT(getMaterialProperty<Real>("kT")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name))
{
}

Real
XeSinkRate::computeQpResidual()
{

  //Xe sink term, not used, under development

  Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;
  Real _dgHXe = 8;
  Real _dgSXe = 0;
  Real _kXe = 0.012564;

  //Concentration terms
  Real DF = (_dgHXe - _kT[_qp] * _dgSXe + _kT[_qp] * poly4Log(_u[_qp],tol,0) - 1 * _kT[_qp] * poly4Log(_CU[_qp],tol,0));

  R1 = _test[_i][_qp] * (_u[_qp] * (_MXe[_qp]) * _kXe * _Zg[_qp] * DF);

  R2 = _test[_i][_qp] * (-_u[_qp] * (_MXe[_qp]) * _kXe * _Zg[_qp] * DF);

  return R1;

}

Real
XeSinkRate::computeQpJacobian()
{

  Real tol = _LogTol[_qp];
  Real R1 = 0.0;
  Real R21 = 0.0;
  Real _dgHXe = 8;
  Real _dgSXe = 0;
  Real _kXe = 0.012564;

  //Concentration terms
  Real DF = (_dgHXe - _kT[_qp] * _dgSXe + _kT[_qp] * poly4Log(_u[_qp],tol,0)
             - 1 * _kT[_qp] * poly4Log(_CU[_qp],tol,0));

  R1 = _test[_i][_qp] * (_u[_qp] * (_MXe[_qp]) * _kXe * _Zg[_qp] * DF);

  return _test[_i][_qp] * (_phi[_j][_qp] * (_MXe[_qp]) * _kXe * _Zg[_qp] * DF)
         + _test[_i][_qp] * (_u[_qp] * (_MXe[_qp]) * _kXe * _Zg[_qp]
                             * (_kT[_qp] * poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                             - 1 * _kT[_qp] * poly4Log(_CU[_qp],tol,1) * (-1 * _phi[_j][_qp]));

}
