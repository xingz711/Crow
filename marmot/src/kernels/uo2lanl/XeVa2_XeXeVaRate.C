#include "XeVa2_XeXeVaRate.h"
#include "MathUtils.h"

template<>
InputParameters validParams<XeVa2_XeXeVaRate>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<MaterialPropertyName>("mob_name", "MXeXe",  "XeXeVa mobility");
  params.addRequiredCoupledVar("c7", "XeXeVa concentration");
  params.addRequiredCoupledVar("c2", "Va concentration");
  params.addRequiredCoupledVar("c1", "Xe concentration");
  params.addParam<std::string>("LogC_name", "LogC",  "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol",  "For PolyLog");

  return params;
}

XeVa2_XeXeVaRate::XeVa2_XeXeVaRate(const InputParameters & parameters) :
    Kernel(parameters),
    _mob_name(getParam<MaterialPropertyName>("mob_name")),
    _MXeXe(getMaterialProperty<Real>(_mob_name)),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c7(coupledValue("c7")),
    _grad_c7(coupledGradient("c7")),
    _kT(getMaterialProperty<Real>("kT")),
    _EBXeXeVa(getMaterialProperty<Real>("EBgXeXeVa")),
    _SBXeXeVa(getMaterialProperty<Real>("SBgXeXeVa")),
    _Zg(getMaterialProperty<Real>("Zg")),
    _LogC_name(getParam<std::string>("LogC_name")),
    _LogC(getMaterialProperty<Real>(_LogC_name)),
    _LogTol_name(getParam<std::string>("LogTol_name")),
    _LogTol(getMaterialProperty<Real>(_LogTol_name)),
    _kappa_cggv(getMaterialProperty<Real>("kappa_cGGV")),
    _kappa_cv(getMaterialProperty<Real>("kappa_cV")),
    _kappa_cg(getMaterialProperty<Real>("kappa_c")),
    _kappa_cgvv(getMaterialProperty<Real>("kappa_cGVV"))
{
}

Real
XeVa2_XeXeVaRate::computeQpResidual()
{

  //Reaction: XeVaVa + Xe <-> XeXeVaVa,


  const Real tol = _LogTol[_qp];
  const Real S = 0.385;

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;

  // Concentration terms

  Real DF = (_EBXeXeVa[_qp] - _kT[_qp]*(_SBXeXeVa[_qp])
             + _kT[_qp] * MathUtils::poly4Log(_c7[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_c1[_qp],tol,0)
             - _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cggv[_qp]*_grad_c7[_qp]-_kappa_cg[_qp]*_grad_c1[_qp]-_kappa_cgvv[_qp]*_grad_u[_qp]);

  R1 = -_test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF);
  R2 = -_test[_i][_qp]*(-_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF);
  R21 = -_grad_test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF2);
  R22 = -_grad_test[_i][_qp]*(-_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF2);

  // Check sign to avoid negative concentrations
  if (_c1[_qp] >= 0 && _u[_qp] >= 0)
    return R1+R21;
  else if (_c1[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1+R21;
  else if (_c1[_qp] < 0 || _u[_qp] < 0)
    return R2+R22;
  else
    return R2+R22;
}

Real
XeVa2_XeXeVaRate::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R2 = 0.0;
  Real R21 = 0.0;
  Real R22 = 0.0;
  Real R1a=0.0;
  Real R21a=0.0;
  Real S=0.385;

  // Concentration terms
  Real DF = (_EBXeXeVa[_qp]-_kT[_qp]*(_SBXeXeVa[_qp])+_kT[_qp] * MathUtils::poly4Log(_c7[_qp],tol,0)-_kT[_qp] * MathUtils::poly4Log(_c1[_qp],tol,0)-_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cggv[_qp]*_grad_c7[_qp]-_kappa_cg[_qp]*_grad_c1[_qp]-_kappa_cgvv[_qp]*_grad_u[_qp]);

  R1a = _test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF);
  R21a = _grad_test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF2);

  R1 = -(_test[_i][_qp]*(_phi[_j][_qp]*_c1[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF)-_test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*(_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1)*_phi[_j][_qp])));
  R2 = -(_test[_i][_qp]*(-_phi[_j][_qp]*_c1[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF)-_test[_i][_qp]*(-_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*(_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1)*_phi[_j][_qp])));
  R21 = -(_grad_test[_i][_qp]*(_phi[_j][_qp]*_c1[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF2)-_grad_test[_i][_qp]*(_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp])*(_kappa_cg[_qp]*_grad_phi[_j][_qp]));
  R22 = -(_grad_test[_i][_qp]*(-_phi[_j][_qp]*_c1[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp]*DF2)-_grad_test[_i][_qp]*(-_c1[_qp]*_u[_qp]*(_MXeXe[_qp]/S/S)*_Zg[_qp])*(_kappa_cg[_qp]*_grad_phi[_j][_qp]));

  // Check sign to avoid negative concentrations
  if (_c1[_qp] >= 0 && _u[_qp] >= 0)
    return R1+R21;
  else if (_c1[_qp] < 0.0 && _u[_qp] < 0.0)
    return R1+R21;
  else if (_c1[_qp] < 0 || _u[_qp] < 0)
    return R2+R22;
  else
    return R2+R22;
}
