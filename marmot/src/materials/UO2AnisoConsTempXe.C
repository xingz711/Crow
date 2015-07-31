#include "UO2AnisoConsTempXe.h"
#include "MathUtils.h"

template<>
InputParameters validParams<UO2AnisoConsTempXe>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<Real>("temp", "Constant temperature in Kelvin");
  params.addParam<Real>("kappa_cg", 1.0, "The kappa parameter for the gas concentration");
  params.addParam<Real>("f0s", 0.125,"The GB energy constant ");
  params.addParam<Real>("tstop", 0.001, "Stop time for grain growth ");
  params.addRequiredParam<Real>("M0GB", "GB mobility constant in m^4/(J-s)");
  params.addRequiredParam<Real>("QGB", "GB energy of migration in eV");
  params.addRequiredParam<Real>("sigma_GB", "GB Surface energy in J/m^2");
  params.addRequiredParam<Real>("HXe", "Xe migration barrier in eV");
  params.addRequiredParam<Real>("D0Xe", "Xe pre-exponential factor in nm^2/s");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

UO2AnisoConsTempXe::UO2AnisoConsTempXe(const InputParameters & parameters) :
    Material(parameters),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _temp(getParam<Real>("temp")),
    _kappa_cg(getParam<Real>("kappa_cg")),
    _f0s(getParam<Real>("f0s")),
    _tstop(getParam<Real>("tstop")),
    _M0GB(getParam<Real>("M0GB")),
    _QGB(getParam<Real>("QGB")),
    _sigma_GB(getParam<Real>("sigma_GB")),
    _HXe(getParam<Real>("HXe")),
    _D0Xe(getParam<Real>("D0Xe")),
    _Dg(declareProperty<Real>("Dg")),
    _Mg(declareProperty<Real>("Mg")),
    _Mgm(declareProperty<Real>("Mgm")),
    _grad_Mg(declareProperty<RealGradient>("grad_Mg")),
    _grad_Mgm(declareProperty<RealGradient>("grad_Mgm")),
    _kT(declareProperty<Real>("kT")),
    _vC_i(declareProperty<std::vector<Real> >("vC_i")),
    _vm_i(declareProperty<std::vector<Real> >("vm_i")),
    _vb_i(declareProperty<std::vector<Real> >("vb_i")),
    _kb(declareProperty<Real>("kb")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _kappa_GB(declareProperty<Real>("kappa_op")),
    _gamma(declareProperty<Real>("gamma_asymm")),
    _L(declareProperty<Real>("L")),
    _mu(declareProperty<Real>("mu")),
    _LogC(declareProperty<Real>("LogC")),
    _LogTol(declareProperty<Real>("LogTol")),
    _tgrad_corr_mult(declareProperty<Real>("tgrad_corr_mult")),
    _n(coupledComponents("v"))
{
  _vals.resize(_n);
  _grad_vals.resize(_n);
  for (unsigned int i = 0; i < _n; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }

  // Convert parameter units
  const Real JtoeV = 6.24150974e18; // Joule to eV conversion
  _sigma_GB *= JtoeV * (1e-9*1e-9); // convert of eV/nm^2
  _M0GB *= 1.0e0 / (JtoeV * (1.e-9*1.e-9*1.e-9*1.e-9)); // Convert to nm^4/(eV*s)
}

void
UO2AnisoConsTempXe::computeProperties()
{

  //Materials file for Xe, U

  Real vC_i[] = { 3.55, 3.55, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 0.64, 4.79, 3.55, 3.55, 4.79, 0.64, 3.55, 3.55 };
  Real vm_i[] = { 0.872, 0.872, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.346, 0.954, 0.872, 0.872, 0.954, 0.346, 0.872, 0.872 };
  Real vb_i[] = { -0.444, -0.444, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.278, 1.485, -0.444, -0.444, 1.485, -0.278, -0.444, -0.444 };
  Real gbw_i[] = { 3.704, 3.704, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 4.128, 3.642, 3.704, 3.704, 3.642, 4.128, 3.704, 3.704 };

  Real D0 = _D0Xe; // nm^2/s
  Real DH0 = _HXe; // eV

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K
    _kT[_qp] = _kb[_qp] * _temp;

    Real f_i = 0.0;
    RealGradient grad_f_i = 0.0;
    Real w_GB = 0.0;
    unsigned int ind=0;
    Real smphi = 0.0;

    for (unsigned int i = 0; i < _n; ++i)
      for (unsigned int j = i + 1; j < _n; ++j)
      {
        ind = i * 4 + (j - 1);

        f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp]
               * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i += -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp]
                    * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        grad_f_i += -8.0       * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                    * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

        w_GB += gbw_i[ind] * (*_vals[i])[_qp] * (*_vals[i])[_qp]
                * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        smphi +=   (*_vals[i])[_qp] * (*_vals[i])[_qp]
                   * (*_vals[j])[_qp] * (*_vals[j])[_qp];
      }

    if (smphi > 0)
      w_GB/=smphi;
    else
      w_GB = gbw_i[0];

    // For gas atoms
    Real DH = DH0;
    RealGradient grad_DH = 0;

    _Dg[_qp] = D0 * std::exp(-DH/_kT[_qp]); // nm^2/s
    _Mg[_qp] = _Dg[_qp] / _kT[_qp]; // nm^2/(s*eV);
    _Mgm[_qp] = -_Dg[_qp] / _kT[_qp]; // nm^2/(s*eV);

    RealGradient grad_D = -D0 * grad_DH * std::exp(-DH / _kT[_qp]);
    _grad_Mg[_qp] = grad_D / _kT[_qp];
    _grad_Mgm[_qp] = -grad_D / _kT[_qp];

    _vC_i[_qp].resize(16);
    _vm_i[_qp].resize(16);
    _vb_i[_qp].resize(16);

    for (unsigned int i = 0; i < 16; ++i)
    {
      _vC_i[_qp][i] = vC_i[i];
      _vm_i[_qp][i] = vm_i[i];
      _vb_i[_qp][i] = vb_i[i];
    }

    _kappa_c[_qp] = _kappa_cg;

    // For GB
    Real M_GB=0.0;
    if (_t>_tstop)
      M_GB = 0.0;
    else
      M_GB = _M0GB * std::exp(-_QGB / (_kb[_qp] * _temp));

    _L[_qp] = 4.0 / 3.0 * M_GB/w_GB;

    _kappa_GB[_qp] = 3.0 / 4.0 * _sigma_GB * w_GB;
    _gamma[_qp] = 1.5;

    _mu[_qp] = 3.0 / 4.0 * 1.0 / _f0s * _sigma_GB / w_GB;
    _tgrad_corr_mult[_qp] = _mu[_qp] * 9.0/8.0;

    // Cutoffs for mathematical functions
    _LogC[_qp] = 1e-12;
    _LogTol[_qp] = 1e-12;
  }
}
