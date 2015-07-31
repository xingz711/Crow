#include "UO2VacGBConsT.h"

template<>
InputParameters validParams<UO2VacGBConsT>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredParam<Real>("temp", "Temperature");
  params.addRequiredParam<Real>("M0_GB", "Grain boundary mobility at T=0 K");
  params.addRequiredParam<Real>("Q", "Q for GB mobility");
  params.addRequiredParam<Real>("l_GB", "GB width");
  params.addRequiredParam<Real>("sigma_GB", "grain boundary energy");
  params.addRequiredParam<Real>("l_SS", "void surface width");
  params.addRequiredParam<Real>("sigma_SS", "void surface energy");
  params.addRequiredParam<Real>("Dv0", "Diffusivity multiplier in nm^2/s");
  params.addRequiredParam<Real>("Evm", "Vacancy energy of migration in eV");
  params.addRequiredCoupledVar("cv", "vacancy concentration");
  params.addParam<Real>("Dg_mult", 1.0, "Mullitplier to calc. Dg from Dv");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");

  return params;
}

UO2VacGBConsT::UO2VacGBConsT(const InputParameters & parameters) :
    Material(parameters),
    _cv(coupledValueOld("cv")),
    _sigma_SS(getParam<Real>("sigma_SS")),
    _sigma_GB(getParam<Real>("sigma_GB")),
    _l_SS(getParam<Real>("l_SS")),
    _l_GB(getParam<Real>("l_GB")),
    _temp(getParam<Real>("temp")),
    _M0_GB(getParam<Real>("M0_GB")),
    _Q(getParam<Real>("Q")),
    _kappa(declareProperty<Real>("kappa")),
    _Dg(declareProperty<Real>("Dg")),
    _Dv(declareProperty<Real>("Dv")),
    _grad_Dv(declareProperty<RealGradient>("grad_Dv")),
    _Mv(declareProperty<Real>("Mv")),
    _grad_Mv(declareProperty<RealGradient>("grad_Mv")),
    _L(declareProperty<Real>("L")),
    _Ao(declareProperty<Real>("Ao")),
    _BetaS(declareProperty<Real>("BetaS")),
    _BetaGB(declareProperty<Real>("BetaGB")),
    _mu(declareProperty<Real>("mu")),
    _Dv0(getParam<Real>("Dv0")),
    _Evm(getParam<Real>("Evm")),
    _n(coupledComponents("v"))
{
  // Assign grain order parameters
  _vals.resize(_n);
  for (unsigned int i = 0; i < _n; ++i)
    _vals[i] = &coupledValueOld("v", i);

  // Assign constants
  _kb = 8.617343e-5; // Boltzmann constant in eV/K

  // Compute GB mobility
  _M_GB = _M0_GB * std::exp(-_Q/(_kb*_temp));
}


void
UO2VacGBConsT::computeProperties()
{
  const Real f0s = 0.125;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    Real kappa_GB =  3.0/4.0*_sigma_GB*_l_GB;
    Real kappa_SS = 3.0/4.0*_sigma_SS*_l_SS;

    Real sum_Pi = 0.0;
    Real PiPj = 0.0;
    for (unsigned int i=0; i<_n; ++i)
    {
      sum_Pi += (*_vals[i])[_qp] * (*_vals[i])[_qp];

      for (unsigned int j = i + 1; j < _n; ++j)
        PiPj +=   (*_vals[i])[_qp] * (*_vals[i])[_qp]
                * (*_vals[j])[_qp] * (*_vals[j])[_qp];
    }

    Real Picv = sum_Pi*_cv[_qp]*_cv[_qp];

    if ((PiPj + Picv) > 0.0)
      _kappa[_qp] = (kappa_GB*PiPj + kappa_SS*Picv)/(PiPj + Picv);
    else
      _kappa[_qp] = kappa_GB;

    _L[_qp] = 4.0/3.0 * _M_GB/_l_GB;
    _Ao[_qp] = 3.0/4.0 * 1.0/f0s * _sigma_GB/_l_GB;
    _BetaS[_qp] = kappa_SS / 2.0;
    _BetaGB[_qp] = kappa_GB / 2.0;

    _mu[_qp] = 0.3;

    _Dv[_qp] = _Dv0 * std::exp(-_Evm/(_kb*_temp));
    _grad_Dv[_qp] = 0.0;

    _Mv[_qp] = _Dv[_qp]/(_Ao[_qp]*(2.0*_BetaS[_qp] - 1.0));
    // _Mv[_qp] = _Dv[_qp]/((2.0*_BetaS[_qp] - 1.0));
    _grad_Mv[_qp] = 0.0;

    _Dg[_qp] = _Dv[_qp]*_Dg_mult;
  }
}
