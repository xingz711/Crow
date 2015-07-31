#include "PoreGBMaterial.h"

template<>
InputParameters validParams<PoreGBMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addRequiredParam<Real>("int_width", "The interfacial width in the lengthscale of the problem");
  params.addParam<Real>("length_scale", 1.0e-9, "defines the base length scale of the problem in m");
  params.addParam<Real>("time_scale", 1.0e-9, "defines the base time scale of the problem");
  params.addRequiredParam<Real>("D0", "Diffusivity prefactor for vacancies in m^2/s");
  params.addRequiredParam<Real>("Em", "Vacancy migration energy in eV");
  params.addParam<Real>("en_ratio", 1.0, "ratio of surface energy to GB energy");
  params.addRequiredParam<Real>("GB_energy", "GB energy in J/m2");
  params.addParam<Real>("GBmob0", 0.0, "Grain boundary mobility prefactor in m^4/(J*s)");
  params.addParam<Real>("Q", 0.0, "Grain boundary migration activation energy in eV");
  params.addParam<Real>("GBMobility", -1, "GB mobility input in m^4/(J*s), that overrides the temperature dependent calculation");
  return params;
}

PoreGBMaterial::PoreGBMaterial(const InputParameters & parameters) :
    Material(parameters),
    _T(coupledValue("T")),
    _M_c(declareProperty<Real>("M_c")),
    _grad_M_c(declareProperty<RealGradient>("grad_M_c")),
    _D(declareProperty<Real>("D")),
    _L(declareProperty<Real>("L")),
    _kappa_c(declareProperty<Real>("kappa_c")),
    _kappa_GB(declareProperty<Real>("kappa_op")),
    _mu(declareProperty<Real>("mu")),
    _gamma(declareProperty<Real>("gamma_asymm")),
    _tgrad_corr_mult(declareProperty<Real>("tgrad_corr_mult")),
    _int_width(getParam<Real>("int_width")),
    _length_scale(getParam<Real>("length_scale")),
    _time_scale(getParam<Real>("time_scale")),
    _D0(getParam<Real>("D0")),
    _Em(getParam<Real>("Em")),
    _GB_energy(getParam<Real>("GB_energy")),
    _GBmob0(getParam<Real>("GBmob0")),
    _Q(getParam<Real>("Q")),
    _en_ratio(getParam<Real>("en_ratio")),
    _GBMobility(getParam<Real>("GBMobility")),
    _JtoeV(6.24150974e18), // Joule to eV conversion
    _kb(8.617343e-5) // Boltzmann constant in eV/K
{
 if (_GBMobility == -1 && _GBmob0 == 0)
   mooseError("Either a value for GBMobility or for GBmob0 and Q must be provided");
}

void
PoreGBMaterial::computeProperties()
{
  const Real D0_c = _D0*_time_scale / (_length_scale*_length_scale); // Convert mobility from m^2/s to the length and time scale
  const Real int_width_c = _int_width; // The interfacial width is input in the length scale of the problem, so no conversion is necessary
  const Real GB_energy = _GB_energy * _JtoeV * _length_scale*_length_scale; // Convert surface energy from J/m2 to eV/length_scale^2
  const Real f0s = 1.0/8.0;
  const Real GBmob0_c = _GBmob0*_time_scale/(_JtoeV*(_length_scale*_length_scale*_length_scale*_length_scale)); // Convert to lengthscale^4/(eV*timescale);

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _D[_qp] = D0_c * std::exp(-_Em / (_kb * _T[_qp]));
    _kappa_c[_qp] = 3.0/4.0 * GB_energy * _en_ratio * int_width_c;
    _kappa_GB[_qp] = 3.0/4.0 * GB_energy * int_width_c;

    _gamma[_qp] = 1.5;
    Real gamma_c = _gamma[_qp] * _en_ratio;

    _mu[_qp] = 3.0/4.0 * 1.0/f0s * GB_energy / int_width_c;
    _tgrad_corr_mult[_qp] = _mu[_qp] * 9.0/8.0;
    _M_c[_qp] = _D[_qp] / (2.0 * _mu[_qp] * (gamma_c - 1.0));
    _grad_M_c[_qp] = 0.0;

    Real GBmob;
    if (_GBMobility < 0)
      GBmob = GBmob0_c * std::exp(-_Q / (_kb * _T[_qp]));
    else
    {
      // Convert to lengthscale^4/(eV*timescale);;
      GBmob = _GBMobility * _time_scale / (_JtoeV * (_length_scale*_length_scale*_length_scale*_length_scale));
    }

    _L[_qp] = 4.0/3.0 * GBmob / int_width_c;
  }
}
