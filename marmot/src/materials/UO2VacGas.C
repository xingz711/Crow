#include "UO2VacGas.h"

template<>
InputParameters validParams<UO2VacGas>()
{
  InputParameters params = validParams<Material>();

  params.addParam<Real>("temp", 0.0, "Constant temperature in Kelvin");
  params.addParam<PostprocessorName>("coupled_temp_name", "dummy", "The name of the coupled temperature");
  params.addParam<bool>("coupled_temp", false, "Whether or not the temperature should come from a coupled Bison run.");
  params.addRequiredCoupledVar("cv", "Vacancy Concentration");
  params.addRequiredCoupledVar("cg", "Gas Concentration");
  params.addRequiredParam<Real>("int_width", "The interfacial width of void surface in the lengthscale of the problem");
  params.addParam<Real>("length_scale", 1.0e-8, "defines the base length scale of the problem");
  params.addParam<Real>("time_scale", 1.0, "defines the base time scale of the problem");
  params.addParam<Real>("D0vac", 2.0e-7, "Diffusivity prefactor for vacancies in m^2/s");
  params.addParam<Real>("D0gas", 5.0e-5, "Diffusivity prefactor for gas in m^2/s");
  params.addParam<Real>("Emvac", 2.4, "Vacancy migration energy in eV");
  params.addParam<Real>("Emgas", 3.9, "Gas migration energy in eV");
  params.addParam<Real>("Efvac", 2.69, "Vacancy formation energy in eV");
  params.addParam<Real>("Efgas", 4.35, "Gas formation energy in eV");
  return params;
}

UO2VacGas::UO2VacGas(const InputParameters & parameters) :
    DerivativeMaterialInterface<Material>(parameters),
    _cv(coupledValue("cv")),
    _cv_name(getVar("cv", 0)->name()),
    _cg(coupledValue("cg")),
    _cg_name(getVar("cg", 0)->name()),
    _input_temp(getParam<Real>("temp")),
    _temp(getParam<bool>("coupled_temp") ? getPostprocessorValue("coupled_temp_name") : _input_temp),
    _kT(declareProperty<Real>("kT")),
    _RT(declareProperty<Real>("RT")),
    _M_v(declareProperty<Real>("M_v")),
    _dM_vdcv(declarePropertyDerivative<Real>("M_v", _cv_name)),
    _dM_vdcg(declarePropertyDerivative<Real>("M_v", _cg_name)),
    _d2M_vdcv2(declarePropertyDerivative<Real>("M_v", _cv_name, _cv_name)),
    _d2M_vdcvdcg(declarePropertyDerivative<Real>("M_v", _cv_name, _cg_name)),
    _Ms_v(declareProperty<Real>("Ms_v")),
    _D_v(declareProperty<Real>("D_v")),
    _M_g(declareProperty<Real>("M_g")),
    _dM_gdcg(declarePropertyDerivative<Real>("M_g", _cg_name)),
    _dM_gdcv(declarePropertyDerivative<Real>("M_g", _cv_name)),
    _d2M_gdcg2(declarePropertyDerivative<Real>("M_g", _cg_name, _cg_name)),
    _d2M_gdcgdcv(declarePropertyDerivative<Real>("M_g", _cg_name, _cv_name)),
    _Ms_g(declareProperty<Real>("Ms_g")),
    _Efv(declareProperty<Real>("Efv")),
    _Efg(declareProperty<Real>("Efg")),
    _L_eta(declareProperty<Real>("L_eta")),
    _kb(declareProperty<Real>("kb")),
    _kappa_v(declareProperty<Real>("kappa_v")),
    _kappa_g(declareProperty<Real>("kappa_g")),
    _cv_eq(declareProperty<Real>("cv_eq")),
    _W(declareProperty<Real>("barr_height")),
    _mug0(declareProperty<Real>("mug0")),
    _int_width(getParam<Real>("int_width")),
    _length_scale(getParam<Real>("length_scale")),
    _time_scale(getParam<Real>("time_scale")),
    _D0vac(getParam<Real>("D0vac")),
    _Emvac(getParam<Real>("Emvac")),
    _Efvac(getParam<Real>("Efvac")),
    _D0gas(getParam<Real>("D0gas")),
    _Emgas(getParam<Real>("Emgas")),
    _Efgas(getParam<Real>("Efgas"))
{
}

void
UO2VacGas::computeProperties()
{
  const Real tol = 1.0e-8;

  Real Dv0 = _D0vac*_time_scale/(_length_scale*_length_scale); // (10nm)^2/s Matzke1986
  Real Emv = _Emvac; // Ev

  Real Dg0 = _D0gas * _time_scale / (_length_scale*_length_scale);
  Real Emg = _Emgas; // Ev

  Real conv_int_width = _int_width;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // Assign variables
    Real cv = _cv[_qp];
    Real cg = _cg[_qp];

    _mug0[_qp] = 1.4;
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K

    _kT[_qp] = _kb[_qp]*_temp;
    _RT[_qp] = _kb[_qp]*_temp;

    _Efv[_qp] = _Efvac; // eV
    _Efg[_qp] = _Efgas; // eV

    _cv_eq[_qp] = std::exp(-_Efv[_qp]/_kT[_qp]);
    _D_v[_qp] = Dv0 * std::exp(-Emv/_kT[_qp]);

    Real Dg = Dg0 * std::exp(-Emg/_kT[_qp]);

    Real Wlog = _kT[_qp] * std::log(0.5) + 0.25 * _Efv[_qp];

    Real kappa_ref = 1.00;
    const Real M = 1.10880281222080778746886;
    Real surf_energy = M * std::sqrt(kappa_ref*Wlog);
    _kappa_v[_qp] = std::sqrt(2.0) * surf_energy * conv_int_width / M;

    Wlog = _kT[_qp] * std::log(0.5) + 0.25 * _Efg[_qp];
    surf_energy = M * std::sqrt(kappa_ref * Wlog);
    _kappa_g[_qp] = std::sqrt(2.0) * surf_energy * conv_int_width / M;

    _W[_qp] = surf_energy/(conv_int_width*M*std::sqrt(2.0));

    if (cv < tol)
      cv = tol;
    if (cv > 1.0 - tol)
      cv = 1.0 - tol;
    if (cg < tol)
      cg = tol;
    if (cg > 1.0 - tol)
      cg = 1.0 - tol;

    _Ms_v[_qp] = _D_v[_qp] / _kT[_qp];
    _M_v[_qp] = _Ms_v[_qp] * (cv - cv*cv - cv*cg);
    _dM_vdcv[_qp] = _Ms_v[_qp] * (1.0 - 2.0*cv - cg);
    _dM_vdcg[_qp] = -_Ms_v[_qp] * cv;
    _d2M_vdcv2[_qp] = -_Ms_v[_qp] * 2.0;
    _d2M_vdcvdcg[_qp] = -_Ms_v[_qp];
    _L_eta[_qp] = _Ms_v[_qp];

    _Ms_g[_qp] = Dg / _kT[_qp];
    if (cv > 0.8)
      _Ms_g[_qp] = 100.0 * Dg / _kT[_qp];

    _M_g[_qp] = _Ms_g[_qp] * (cg - cg*cg - cv*cg);
    _dM_gdcg[_qp] = _Ms_g[_qp] * (1.0 - 2.0*cg - cv);
    _dM_gdcv[_qp] = -_Ms_g[_qp] * cg;
    _d2M_gdcg2[_qp] = -_Ms_g[_qp] * 2.0;
    _d2M_gdcgdcv[_qp] = -_Ms_g[_qp];

    if (_M_v[_qp] < tol)
      _M_v[_qp] = tol;
    if (_M_g[_qp] < tol)
      _M_g[_qp] = tol;
  }
}
