#include "PoreGBMtrxMobMaterial.h"

template<>
InputParameters validParams<PoreGBMtrxMobMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
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
  params.addRequiredCoupledVar("c", "intermediate parameter--concentration");
  params.addParam<Real>("surfindex", 0.0, "Index for surface diffusion");
  params.addParam<Real>("gbindex", 0.0, "Index for GB diffusion");
  params.addParam<Real>("bulkindex", 1.0, "Index for bulk diffusion");
  return params;
}

PoreGBMtrxMobMaterial::PoreGBMtrxMobMaterial(const InputParameters & parameters) :
    Material(parameters),
    _T(coupledValue("T")),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _M_c(declareProperty<RealTensorValue>("M_c")),
    _dMdc(declareProperty<RealTensorValue>("dMdc")),
    _sM(declareProperty<Real>("sM")),
    _L(declareProperty<Real>("L")),
    _l_GB(declareProperty<Real>("l_GB")),
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
    _surfindex(getParam<Real>("surfindex")),
    _gbindex(getParam<Real>("gbindex")),
    _bulkindex(getParam<Real>("bulkindex")),
    _JtoeV(6.24150974e18), // Joule to eV conversion
    _kb(8.617343e-5), // Boltzmann constant in eV/K
    _ncrys(coupledComponents("v"))
{
  if (_GBMobility == -1 && _GBmob0 == 0)
    mooseError("Either a value for GBMobility or for GBmob0 and Q must be provided");

  if (_ncrys == 0)
    mooseError("Model requires op_num > 0");

  _vals.resize(_ncrys);
  _grad_vals.resize(_ncrys);
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

void
PoreGBMtrxMobMaterial::computeProperties()
{
  RealTensorValue I(1,0,0,0,1,0,0,0,1);

  Real D0_c = _D0 * _time_scale / (_length_scale*_length_scale); // Convert mobility from m^2/s to the length and time scale
  Real int_width_c = _int_width;  // The interfacial width is input in the length scale of the problem, so no conversion is necessary

  // Convert surface energy from J/m2 to eV/length_scale^2
  Real GB_energy = _GB_energy * _JtoeV * _length_scale*_length_scale;

  Real f0s = 1.0/8.0;

  Real GBmob0_c = _GBmob0 * _time_scale / (_JtoeV * (_length_scale*_length_scale*_length_scale*_length_scale)); // Convert to lengthscale^4/(eV*timescale);

  for (_qp=0; _qp < _qrule->n_points(); ++_qp)
  {
    Real c = _c[_qp];
    Real mc = 1.0 - _c[_qp];

    // Compute grain boundary diffusivity
    RealTensorValue Dgb(0.0);

    for (unsigned int i = 0; i < _ncrys; ++i)
      for (unsigned int j = 0; j < _ncrys; ++j)
      {
        if (i != j)
        {
          RealGradient ngb = (*_grad_vals[i])[_qp] - (*_grad_vals[j])[_qp];
          if (ngb.size() > 1.0e-10)
            ngb /= ngb.size();
          else
            ngb = 0.0;

          RealTensorValue Tgb;

          for (unsigned int a = 0; a < 3; ++a)
            for (unsigned int b = 0; b < 3; ++b)
              Tgb(a,b) = I(a,b) - ngb(a) * ngb(b);

          Dgb += (*_vals[i])[_qp] * (*_vals[j])[_qp] * Tgb;
        }
      }

    // Compute surface diffusivity matrix
    RealGradient ns(0), dns(0);

    if (_grad_c[_qp].size() > 1.0e-10)
      ns = _grad_c[_qp]/_grad_c[_qp].size();

    RealTensorValue Ts;
    RealTensorValue dTs;

    for (unsigned int a = 0; a < 3; ++a)
      for (unsigned int b = 0; b < 3; ++b)
      {
        Ts(a,b) = I(a,b) - ns(a) * ns(b);
        dTs(a,b) = - 2.0 * dns(a) * ns(b);
      }

    RealTensorValue Dsurf = c*c * mc*mc * Ts;

    RealTensorValue dDsurfdc = (2.0 * c * mc*mc - 2.0 * c*c * mc) * Ts + c*c * mc*mc * dTs;

    // Compute bulk properties
    Real Dbulk = D0_c * std::exp(-_Em / (_kb * _T[_qp]));

    const Real mult_bulk = 1.0;
    const Real dmult_bulk = 0.0;

    // Compute concentration mobility
    RealTensorValue D = Dbulk * (_bulkindex * mult_bulk * I + _gbindex * Dgb + _surfindex * Dsurf);
    RealTensorValue dDdc = Dbulk * (_bulkindex*dmult_bulk * I + _surfindex * dDsurfdc);

    // Interfacial parameters
    _kappa_c[_qp] = 3.0/4.0 * GB_energy * _en_ratio * int_width_c;
    _kappa_GB[_qp] = 3.0/4.0 * GB_energy * int_width_c;

    _gamma[_qp] = 1.5;
    Real gamma_c = _gamma[_qp] * _en_ratio;

    _mu[_qp] = 3.0/4.0 * 1.0 / f0s * GB_energy / int_width_c;
    _tgrad_corr_mult[_qp] = _mu[_qp] * 9.0/8.0;

    _sM[_qp] = Dbulk / (2.0 * _mu[_qp] * (gamma_c - 1.0));
    _M_c[_qp] = D / (2.0 * _mu[_qp] * (gamma_c - 1.0));
    _dMdc[_qp] = dDdc / (2.0 * _mu[_qp] * (gamma_c - 1.0));

    Real GBmob;
    if (_GBMobility < 0)
      GBmob = GBmob0_c * std::exp(-_Q / (_kb * _T[_qp]));
    else
      GBmob = _GBMobility * _time_scale / (_JtoeV * (_length_scale*_length_scale*_length_scale*_length_scale)); // Convert to lengthscale^4/(eV*timescale);;

    _L[_qp] = 4.0/3.0 * GBmob / int_width_c;
    _l_GB[_qp] = _int_width;
  }
}
