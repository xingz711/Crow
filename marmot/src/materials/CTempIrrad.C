#include "CTempIrrad.h"

template<>
InputParameters validParams<CTempIrrad>()
{
  InputParameters params = validParams<Material>();
  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addRequiredCoupledVar("cv","Concentration");
  params.addRequiredParam<Real>("int_width","The interfacial width of void surface in nm");
  params.addParam<PostprocessorName>("coupled_temp_name","","The name of the coupled temperature");
  params.addParam<Real>("length_scale",10.0,"defines the base length scale of the problem in nm");
  params.addParam<Real>("time_scale",1.0,"defines the base time scale of the problem");
  params.addRequiredParam<unsigned int>("free_energy_form", "If 0, physical free energy, then polynomial order");
  params.addParam<Real>("D0vac",2.0e9,"Diffusivity prefactor for vacancies in (10nm)^2/s");
  params.addParam<Real>("Emvac",2.4,"Vacancy migration energy in eV");
  params.addParam<Real>("Efvac",2.69,"Vacancy formation energy in eV");
  params.addParam<Real>("D0int",1.0e6,"Diffusivity prefactor for interstitials in (10nm)^2/s");
  params.addParam<Real>("Emint",1.0,"Interstitial migration energy in eV");
  params.addParam<Real>("Efint",2.69,"Interstitial formation energy in eV");
  params.addParam<Real>("surface_energy",0.0,"Surface energy in J/m2");
  return params;
}

CTempIrrad::CTempIrrad(const InputParameters & parameters) :
    DerivativeMaterialInterface<Material>(parameters),
    _cv(coupledValue("cv")),
    _cv_name(getVar("cv", 0)->name()),
    _coupled_temp(getParam<PostprocessorName>("coupled_temp_name") != ""),
    _temp(_coupled_temp ? &getPostprocessorValue("coupled_temp_name") : NULL),
    _T(coupledValue("T")),
    _kT(declareProperty<Real>("kT")),
    _M_v(declareProperty<Real>("M_v")),
    _Ms_v(declareProperty<Real>("Ms_v")),
    _D_v(declareProperty<Real>("D_v")),
    _dM_vdcv(declarePropertyDerivative<Real>("M_v", _cv_name)),
    _d2M_vdcv2(declarePropertyDerivative<Real>("M_v", _cv_name, _cv_name)),
    _M_i(declareProperty<Real>("M_i")),
    _Efv(declareProperty<Real>("Efv")),
    _Efi(declareProperty<Real>("Efi")),
    _L_eta(declareProperty<Real>("L_eta")),
    _kb(declareProperty<Real>("kb")),
    _kappa_v(declareProperty<Real>("kappa_v")),
    _cv_eq(declareProperty<Real>("cv_eq")),
    _W(declareProperty<Real>("barr_height")),
    _int_width(getParam<Real>("int_width")),
    _length_scale(getParam<Real>("length_scale")),
    _conv_f(10.0/_length_scale),
    _time_scale(getParam<Real>("time_scale")),
    _free_energy_form(getParam<unsigned int>("free_energy_form")),
    _D0vac(getParam<Real>("D0vac")),
    _Emvac(getParam<Real>("Emvac")),
    _Efvac(getParam<Real>("Efvac")),
    _D0int(getParam<Real>("D0int")),
    _Emint(getParam<Real>("Emint")),
    _Efint(getParam<Real>("Efint")),
    _surface_energy(getParam<Real>("surface_energy"))
{
}

void
CTempIrrad::computeProperties()
{
  Real tol = 1.0e-10;
  tol = 0.0;

  Real Dv0 = _D0vac * _conv_f*_conv_f * _time_scale; // (10nm)^2/s Matzke1986
  Real Emv = _Emvac; // Ev

  Real Di0 = _D0int * _conv_f*_conv_f * _time_scale;
  Real Emi = _Emint; // Ev

  Real conv_int_width = _int_width / _length_scale;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    // _mug0[_qp] = 1.4;
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K

    // _kT is in units of eV
    if (_coupled_temp)
      _kT[_qp] = _kb[_qp] * (*_temp);
    else
      _kT[_qp] = _kb[_qp] * _T[_qp];

    _Efv[_qp] = _Efvac; // eV
    _Efi[_qp] = _Efint; // eV

    _cv_eq[_qp] = std::exp(-_Efv[_qp] / _kT[_qp]);
    _L_eta[_qp] = 1.0;
    _D_v[_qp] = Dv0 * std::exp(-Emv / _kT[_qp]);

    Real Di = Di0 * std::exp(-Emi / _kT[_qp]);
    Real Wlog = _kT[_qp] * std::log(0.5) + 0.25 * _Efv[_qp];
    Real kappa_ref = 1.00;
    Real Mlog = 1.10880281222080778746886;
    Real M = 0.0;

    if (_free_energy_form == 0)
      M = Mlog;
    if (_free_energy_form == 4)
      M = 2.0/3.0 * std::sqrt(2.0);
    else if (_free_energy_form == 6)
      M =   3.0/16.0 * std::sqrt(2.0) * std::sqrt(3.0)
          - 9.0/32.0 * std::log(-std::sqrt(2.0) + std::sqrt(3.0))
          + 9.0/32.0 * std::log(std::sqrt(2.0) + std::sqrt(3.0));
    else if (_free_energy_form == 8)
      M = 1.181590213;

    Real surf_energy = Mlog * std::sqrt(kappa_ref * Wlog);

    _kappa_v[_qp] = std::sqrt(2.0) * surf_energy * conv_int_width / M;
    _W[_qp] = surf_energy / (conv_int_width * M * std::sqrt(2.0));

    if (_free_energy_form == 0) // Natural log free energy
    {
      Real cv = _cv[_qp];

      if (cv < tol)
        cv = tol;
      if (cv > 1.0 - tol)
        cv = 1.0 - tol;

      _M_v[_qp] = _D_v[_qp]*( cv - cv*cv )/_kT[_qp];
      _dM_vdcv[_qp] = _D_v[_qp]*( 1.0 - 2.0*cv )/_kT[_qp];
      _d2M_vdcv2[_qp] = _D_v[_qp]*2.0/_kT[_qp];
      _Ms_v[_qp] = _D_v[_qp]/_kT[_qp];
    }
    else // Polynomial free energies
    {
      Real Co = 0.0;

      if (_free_energy_form == 4) // 4th order polynomial
        Co = 1.0/32.0;
      else if (_free_energy_form == 6) // 6th order polynomial
      {
        Real a = 0.5;
        Co = 1.0 / (1536.0 * a*a*a*a);
      }
      else if (_free_energy_form == 8) // 8th order polynomial
        Co = 1.0/192.0;

      _M_v[_qp] = Co * _D_v[_qp] * std::sqrt(2.0) * conv_int_width * M / surf_energy;
      _dM_vdcv[_qp] = 0.0;
      _d2M_vdcv2[_qp] = 0.0;
    }

    _M_i[_qp] = Di / _kT[_qp];
  }
}
