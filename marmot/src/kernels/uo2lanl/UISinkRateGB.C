#include "UISinkRateGB.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UISinkRateGB>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<std::string>("MUI_name", "MUI", "UI mobility");
  params.addParam<MaterialPropertyName>("mob_name", "M", "The mobility used with the kernel");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addParam<std::string>("LogC_name", "LogC", "For LogTol");
  params.addParam<std::string>("LogTol_name", "LogTol", "For LogTol");
  return params;
}

UISinkRateGB::UISinkRateGB(const InputParameters & parameters) :
    Kernel(parameters),
    _vUIC_i(getMaterialProperty<std::vector<Real> >("vUIC_i")),
    _vUIm_i(getMaterialProperty<std::vector<Real> >("vUIm_i")),
    _vUIb_i(getMaterialProperty<std::vector<Real> >("vUIb_i")),
    _MUI_name(getParam<std::string>("MUI_name")),
    _MUI(getMaterialProperty<Real>(_MUI_name)),
    _CU(getMaterialProperty<Real>("cu")),
    _grad_CU(getMaterialProperty<RealGradient>("grad_cu")),
    _kT(getMaterialProperty<Real>("kT")),
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
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);

  for (unsigned int i=0; i<_n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

Real
UISinkRateGB::computeQpResidual()
{

  //Explicit model of grain boundary as UI sink, not used

  const Real tol = _LogTol[_qp];

  Real R1 = 0.0;
  Real R21 = 0.0;

  Real outval1 = 0.0;
  Real outval2 = 0.0;

  Real ytot = _u[_qp];

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                 * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]);

      Real g_i = _vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i, tol, 0) + _vUIb_i[_qp][ind];

      outval1 += 2.0 * (_vUIC_i[_qp][ind] - g_i) * f_i;
      outval2 += f_i;

      ind++;
    }

  // Concentration terms
  Real DF =   outval1 + _dgHVaUI[_qp] - (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp])
              + _kT[_qp] * MathUtils::poly4Log(_u[_qp], tol, 0)
              - 1.0 *_kT[_qp] * MathUtils::poly4Log(1-_u[_qp], tol, 0);

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp]* (-_grad_u[_qp]));

  R1 = _test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * DF);
  R21 = _grad_test[_i][_qp] * (_u[_qp] * (_MUI[_qp]) * DF2);

  return (R1 + R21) * (-2 * outval2);

}

Real
UISinkRateGB::computeQpJacobian()
{
  const Real tol = _LogTol[_qp];

  Real outval1 = 0.0;
  Real outval2 = 0.0;
  Real doutval1 = 0.0;

  Real ytot = _u[_qp];

  unsigned int ind = 0;
  for (unsigned int i = 0; i < _n; ++i)
    for (unsigned int j = i + 1; j < _n; ++j)
    {
      Real f_i = -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                 * (*_vals[j])[_qp] * (*_vals[j])[_qp];

      Real inside_g_i = 100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind]);

      Real g_i = _vUIm_i[_qp][ind] * MathUtils::poly4Log(inside_g_i,tol,0) + _vUIb_i[_qp][ind];

      outval1 += 2.0 * (_vUIC_i[_qp][ind] - g_i) * f_i;
      doutval1 +=   -_vUIm_i[_qp][ind] * 100.0 * _phi[_j][_qp]
                    * MathUtils::poly4Log((100.0 * ytot + std::exp(-_vUIb_i[_qp][ind]/_vUIm_i[_qp][ind])), tol, 1) * f_i;
      outval2 += f_i;

      ind++;
    }

  // Concentration terms
  Real DF = (outval1 + _dgHVaUI[_qp] - (_dgHVa[_qp] - _kT[_qp] * _dgSVa[_qp]) + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,0) - 1 * _kT[_qp] * MathUtils::poly4Log(1 - _u[_qp],tol,0));

  // Gradient terms
  RealGradient DF2 = (_kappa_cui[_qp] * _grad_u[_qp] - 1 * _kappa_cui[_qp] * (-_grad_u[_qp]));

  return (   _test[_i][_qp] * (_phi[_j][_qp] * _MUI[_qp] * DF)
             + _test[_i][_qp] * (_u[_qp] * _MUI[_qp] * (doutval1 + _kT[_qp] * MathUtils::poly4Log(_u[_qp],tol,1) * _phi[_j][_qp])
                                 -1.0 * _kT[_qp] * MathUtils::poly4Log(1.0 - _u[_qp], tol, 1) * (-1.0 * _phi[_j][_qp]))
             + _grad_test[_i][_qp] * (_phi[_j][_qp] * _MUI[_qp] * DF2)
             + _grad_test[_i][_qp] * (_u[_qp] * _MUI[_qp] * _kappa_cui[_qp] * _grad_phi[_j][_qp]
                                      - 1.0 * _kappa_cui[_qp] * (-1*_grad_phi[_j][_qp])))
         * (-2.0 * outval2);

}
