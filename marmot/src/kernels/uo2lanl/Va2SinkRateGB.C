#include "Va2SinkRateGB.h"
#include "MathUtils.h"
using namespace MathUtils;

template<>
InputParameters validParams<Va2SinkRateGB>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MVa2_name", "MVa2", "VaVa mobility");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredCoupledVar("c1", "Xe Concentration");
  params.addRequiredCoupledVar("c2", "Va Concentration");
  params.addRequiredCoupledVar("c3", "XeVa Concentration");
  params.addParam<std::string>("LogC_name", "LogC", "For PolyLog");
  params.addParam<std::string>("LogTol_name", "LogTol", "For PolyLog");
  return params;
}

Va2SinkRateGB::Va2SinkRateGB(const InputParameters & parameters) :
    Kernel(parameters),
    _vVaC_i(getMaterialProperty<std::vector<Real> >("vVaC_i")),
    _vVam_i(getMaterialProperty<std::vector<Real> >("vVam_i")),
    _vVab_i(getMaterialProperty<std::vector<Real> >("vVab_i")),
    _MVa2_name(getParam<std::string>("MVa2_name")),
    _MVa2(getMaterialProperty<Real>(_MVa2_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _kT(getMaterialProperty<Real>("kT")),
    _c1(coupledValue("c1")),
    _grad_c1(coupledGradient("c1")),
    _c2(coupledValue("c2")),
    _grad_c2(coupledGradient("c2")),
    _c3(coupledValue("c3")),
    _grad_c3(coupledGradient("c3")),
    _dgHVa2(getMaterialProperty<Real>("dgHVa2")),
    _dgSVa2(getMaterialProperty<Real>("dgSVa2")),
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
    _kappa_cu(getMaterialProperty<Real>("kappa_cU")),
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (unsigned int i = 0; i < _n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

Real
Va2SinkRateGB::computeQpResidual()
{
  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R21 = 0.0;

  // Contribution from GBs
  unsigned int ind = 0;
  Real outval1 = 0.0;
  Real outval2 = 0.0;

  Real ytot = 2.0 * _u[_qp] + _c1[_qp] + _c2[_qp] + 2.0 * _c3[_qp];

  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                 * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind]);

      Real g_i = _vVam_i[_qp][ind] * MathUtils::poly4Log(inside_g_i, tol, 0) + _vVab_i[_qp][ind];

      outval1 += 2.0 * 2.0 * (_vVaC_i[_qp][ind] - g_i) * f_i;

      outval2 += f_i;

      ind++;
    }

  // Concentration terms
  Real DF = (outval1 + _dgHVa[_qp] * 2 + 1 * _EBVaVa[_qp] - _kT[_qp]
             * (_dgSVa[_qp] * 2 + 1 * _SBVaVa[_qp]) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cvv[_qp] * _grad_u[_qp] - 2.0 * _kappa_cu[_qp] * _grad_CU[_qp]);

  R1 = _test[_i][_qp] * (_u[_qp] * _MVa2[_qp] * DF);

  R21 = _grad_test[_i][_qp] * (_u[_qp] * _MVa2[_qp] * DF2);

  return (R1 + R21) * (-2 * outval2);

}

Real
Va2SinkRateGB::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  unsigned int ind = 0;
  Real outval1 = 0.0;
  Real outval2 = 0.0;
  Real doutval1 = 0.0;

  Real ytot = 2.0 * _u[_qp] + _c1[_qp] + _c2[_qp] + 2.0 * _c3[_qp];

  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {

      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind]);

      Real g_i = _vVam_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vVab_i[_qp][ind];

      outval1 += 2 * 2 * (_vVaC_i[_qp][ind] - g_i) * f_i;

      doutval1 += 2 * 2 * (-_vVam_i[_qp][ind] * 100 * 2 * _phi[_j][_qp] * MathUtils::poly4Log((100 * ytot + std::exp(-_vVab_i[_qp][ind] / _vVam_i[_qp][ind])),tol,1) * f_i);

      outval2 += f_i;

      ind++;
    }

  // Concentration terms
  Real DF = (outval1 +_dgHVa[_qp] * 2 + 1 * _EBVaVa[_qp] -
             _kT[_qp]*(_dgSVa[_qp]*2+_SBVaVa[_qp])+_kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0)
             - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cvv[_qp] * _grad_u[_qp] - 2 * _kappa_cu[_qp] * _grad_CU[_qp]);

  return (_test[_i][_qp] * (_phi[_j][_qp] * (_MVa2[_qp]) * DF)
          + _test[_i][_qp] * (_u[_qp] * (_MVa2[_qp]) * (doutval1 + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                              - 2 * _kT[_qp] * MathUtils::poly4Log(_CU[_qp],tol,1) * (-2 * _phi[_j][_qp])) + _grad_test[_i][_qp] * (_phi[_j][_qp] * (_MVa2[_qp]) * DF2)
          + _grad_test[_i][_qp] * (_u[_qp] * (_MVa2[_qp]) * _kappa_cvv[_qp] * (_grad_phi[_j][_qp]) - 2 * _kappa_cu[_qp] * (-2 * _grad_phi[_j][_qp])))* (-2 * outval2);

}
