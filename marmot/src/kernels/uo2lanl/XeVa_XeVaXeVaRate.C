#include "XeVa_XeVaXeVaRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<XeVa_XeVaXeVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MXe",  "XeVa mobility");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  params.addRequiredCoupledVar("c7", "XeXeVaVa concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");

  return params;
}

XeVa_XeVaXeVaRate::XeVa_XeVaXeVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MXe(getMaterialProperty<Real>(_mob_name)),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _c7(coupledValue("c7")),
    _grad_c7(coupledGradient("c7")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeVaXeVa(getMaterialProperty<Real>("EBgXeVaXeVa")),
    _SBXeVaXeVa(getMaterialProperty<Real>("SBgXeVaXeVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cggv(getMaterialProperty<Real>("kappa_cGGV")),
    _kappa_cgv(getMaterialProperty<Real>("kappa_cGV"))
{
}

Real
XeVa_XeVaXeVaRate::computeQpResidual()
{

  //Reaction: XeVa+XeVa <-> XeXeVaVa

  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms
  Real DF = (_EBXeVaXeVa[_qp] - _kT[_qp] * (_SBXeVaXeVa[_qp])
             + _kT[_qp] * MathUtils::poly4Log(_c7[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cggv[_qp] * _grad_c7[_qp]
                      - _kappa_cgv[_qp] * _grad_u[_qp]
                      - _kappa_cgv[_qp]*_grad_u[_qp]);

  R1 = -2 * _test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF);

  R2 = -2 * _test[_i][_qp] * (-_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF);

  R21 = -2 * _grad_test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF2);

  R22 = -2 * _grad_test[_i][_qp] * (-_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF2);

  // Check sign to avoid negative concentrations
  if (_u[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}

Real
XeVa_XeVaXeVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;
  Real R1a=0.0;
  Real R21a = 0.0;
  Real S = 0.385;

  // Concentration terms
  Real DF = (_EBXeVaXeVa[_qp] - _kT[_qp] * (_SBXeVaXeVa[_qp])
             + _kT[_qp] * MathUtils::poly4Log(_c7[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cggv[_qp] * _grad_c7[_qp]
                      - _kappa_cgv[_qp] * _grad_u[_qp]
                      - _kappa_cgv[_qp] * _grad_u[_qp]);

  R1a = -_test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF);

  R21a = -_grad_test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF2);

  R1 = -2 * _test[_i][_qp] * (2.0 * _u[_qp] * _phi[_j][_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF)
       + 2 * _test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp]
                               * (_kT[_qp] * 2.0 * MathUtils::poly4Log(_u[_qp],tol,1)*_phi[_j][_qp]));

  R2 = -2 * _test[_i][_qp] * (-2.0 * _u[_qp] * _phi[_j][_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF)
       + 2 * _test[_i][_qp] * (-_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp]
                               * (_kT[_qp] * 2.0 * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp]));

  R21 = -2 * _grad_test[_i][_qp] * (2.0 * _phi[_j][_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF2)
        + 2 * _grad_test[_i][_qp] * (_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp])
        * (-2.0 * _kappa_cgv[_qp] * _grad_phi[_j][_qp]);

  R22 = -2 * _grad_test[_i][_qp] * (-2.0 * _phi[_j][_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp] * DF2)
        + 2 * _grad_test[_i][_qp] * (-_u[_qp] * _u[_qp] * (_MXe[_qp]/S/S) * _Zg[_qp])
        * (-2.0 * _kappa_cgv[_qp] * _grad_phi[_j][_qp]);

  // Check sign to avoid negative concentrations
  if (_u[_qp] >= 0 && _u[_qp] >= 0)
    return R1 + R21;
  else if (_u[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1 + R21;
  else if (_u[_qp] < 0 || _u[_qp] < 0)
    return R2 + R22;
  else
    return R2 + R22;
}
