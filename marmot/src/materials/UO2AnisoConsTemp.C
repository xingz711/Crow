#include "UO2AnisoConsTemp.h"

template<>
InputParameters validParams<UO2AnisoConsTemp>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<Real>("temp", "Constant temperature in Kelvin");
  params.addParam<Real>("kappa_cg", 1.0, "The kappa parameter for the vacancy concentration");
  params.addParam<Real>("f0s", 0.125, "The GB energy constant ");
  params.addRequiredParam<Real>("M0GB", "GB mobility constant in m^4/(J-s)");
  params.addRequiredParam<Real>("QGB", "GB energy of migration in eV");
  params.addRequiredParam<Real>("sigma_GB", "GB Surface energy in J/m^2");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

UO2AnisoConsTemp::UO2AnisoConsTemp(const InputParameters & parameters) :
    Material(parameters),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _temp(getParam<Real>("temp")),
    _kappa_cg(getParam<Real>("kappa_cg")),
    _f0s(getParam<Real>("f0s")),
    _M0GB(getParam<Real>("M0GB")),
    _QGB(getParam<Real>("QGB")),
    _sigma_GB(getParam<Real>("sigma_GB")),
    _Dg(declareProperty<Real>("Dg")),
    _Mg(declareProperty<Real>("Mg")),
    _grad_Mg(declareProperty<RealGradient>("grad_Mg")),
    _Ef(declareProperty<Real>("Ef")),
    _grad_Ef(declareProperty<RealGradient>("grad_Ef")),
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
  _sigma_GB *= JtoeV * (1e-9*1e-9); // Convert of eV/nm^2
  _M0GB *= 1.0e0 / (JtoeV * (1.e-9*1.e-9*1.e-9*1.e-9)); // Convert to nm^4/(eV*s)
}

void
UO2AnisoConsTemp::computeProperties()
{
  Real vC_i[] = { 4.09, 4.09, 0.97, 0.97, 6.42, 6.42 };
  Real vm_i[] = { 0.7, 0.7, 0.171, 0.171, 0.9, 0.9 };
  Real vb_i[] = { 0.9, 0.9, 0.315, 0.315, 2.562, 2.562 };
  Real gbw_i[] = { 5.0, 5.0, 3.688, 3.688, 1.876, 1.876 };

  Real D0 = 5.0e13; // nm^2/s
  Real q_i = 0.1;
  Real DQ0 = 3.9; // eV

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K
    _kT[_qp] = _kb[_qp] * _temp;

    _gamma[_qp] = 1.5;

    Real f_i = 0.0;
    RealGradient grad_f_i = 0.0;
    Real w_GB = 0.0;
    unsigned int ind = 0;
    Real smphi = 0.0;

    for (unsigned int i = 0; i < _n; ++i)
      for (unsigned int j = i + 1; j < _n; ++j)
      {
        f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
        grad_f_i += -8.0 * 2.0 * (*_vals[i])[_qp] * (*_grad_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
        grad_f_i += -8.0 * (*_vals[i])[_qp] * (*_vals[i])[_qp] * 2.0 * (*_vals[j])[_qp] * (*_grad_vals[j])[_qp];

        w_GB += gbw_i[ind] * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];
        smphi += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[j])[_qp] * (*_vals[j])[_qp];

        ind++;
      }

    if (smphi > 0)
      w_GB /= smphi;
    else
      w_GB = gbw_i[0];

    if (w_GB > 5.0)
      w_GB = 5.0;

    // For gas atoms
    Real DQ = DQ0 - q_i * f_i;
    RealGradient grad_DQ = -q_i * grad_f_i;

    _Dg[_qp] = D0 * std::exp(-DQ / _kT[_qp]); // nm^2/s
    _Mg[_qp] = _Dg[_qp] / _kT[_qp] * _c[_qp]; // nm^2/(s*eV);

    RealGradient grad_D = -D0 * grad_DQ * std::exp(-DQ/_kT[_qp]);
    _grad_Mg[_qp] = _Dg[_qp] / _kT[_qp] * _grad_c[_qp] + grad_D / _kT[_qp] * _c[_qp];

    _Ef[_qp] = 2.8; // eV
    _grad_Ef[_qp] = 0.0; // eV

    _vC_i[_qp].resize(6);
    _vm_i[_qp].resize(6);
    _vb_i[_qp].resize(6);

    for (unsigned int i = 0; i < 6; ++i)
    {
      _vC_i[_qp][i] = vC_i[i];
      _vm_i[_qp][i] = vm_i[i];
      _vb_i[_qp][i] = vb_i[i];
    }

    _kappa_c[_qp] = _kappa_cg;

    // For GB
    Real M_GB = _M0GB*std::exp(-_QGB/(_kb[_qp]*_temp));
    _L[_qp] = 4.0/3.0*M_GB/w_GB;

    _kappa_GB[_qp] = 3.0/4.0 * _sigma_GB * w_GB;
    _mu[_qp] = 3.0/4.0 * 1.0 / _f0s * _sigma_GB / w_GB;
    _tgrad_corr_mult[_qp] = _mu[_qp] * 9.0/8.0;
  }
}
