#include "CuVacProps.h"

template<>
InputParameters validParams<CuVacProps>()
{
  InputParameters params = validParams<Material>();

  params.addCoupledVar("T", "Temperature variable in Kelvin");
  params.addRequiredCoupledVar("cv", "Concentration");
  params.addRequiredParam<Real>("int_width", "The interfacial width of void surface in the length scale of the model");
  params.addParam<Real>("length_scale", 1.0e-9, "defines the base length scale of the problem in m");
  params.addParam<Real>("time_scale", 1.0e-9, "defines the base time scale of the problem in s");
  params.addRequiredParam<unsigned int>("free_energy_form", "If 0, physical free energy, then polynomial order");
  params.addParam<Real>("en_ratio", 1.0, "Ratio of Surfance energy to GB energy");
  params.addParam<Real>("Dv0in", 3.1e-5, "Diffusion constant in m^2/s, defaults to copper from Brown1980");
  params.addParam<Real>("Emvin", 0.71, "Vacancy migration energy in eV, defaults to copper from Balluffi1978 Table 2");
  params.addParam<Real>("Efvin", 1.28, "Vacancy formation energy in eV, defaults to copper from Balluffi1978 Table 2");
  return params;
}

CuVacProps::CuVacProps(const InputParameters & parameters) :
    Material(parameters),
    _cv(coupledValue("cv")),
    _grad_cv(coupledGradient("cv")),
    _T(coupledValue("T")),
    _kT(declareProperty<Real>("kT")),
    _M_v(declareProperty<Real>("M_v")),
    _Ms_v(declareProperty<Real>("Ms_v")),
    _D_v(declareProperty<Real>("D_v")),
    _grad_M_v(declareProperty<RealGradient>("grad_M_v")),
    _M_i(declareProperty<Real>("M_i")),
    _grad_M_i(declareProperty<RealGradient>("grad_M_i")),
    _Efv(declareProperty<Real>("Efv")),
    _Efi(declareProperty<Real>("Efi")),
    _kb(declareProperty<Real>("kb")),
    _kappa_v(declareProperty<Real>("kappa_v")),
    _cv_eq(declareProperty<Real>("cv_eq")),
    _W(declareProperty<Real>("barr_height")),
    _Qstar(declareProperty<Real>("Qstar")),
    _int_width(getParam<Real>("int_width")),
    _length_scale(getParam<Real>("length_scale")),
    _time_scale(getParam<Real>("time_scale")),
    _free_energy_form(getParam<unsigned int>("free_energy_form")),
    _en_ratio(getParam<Real>("en_ratio")),
    _Dv0in(getParam<Real>("Dv0in")),
    _Emvin(getParam<Real>("Emvin")),
    _Efvin(getParam<Real>("Efvin"))
{
}

void
CuVacProps::computeProperties()
{
  const Real JtoeV = 6.24150974e18; // Joule to eV conversion

  Real Dv0 = _Dv0in;
  Dv0 *= _time_scale / (_length_scale*_length_scale); // Convert units
  const Real Emv = _Emvin; // Ev // suziki2003

  Real Di0 = 4.0e-5; // Need value
  Di0 *= _time_scale / (_length_scale*_length_scale);
  const Real Emi = 0.098; // Ev

  Real surf_energy = 0.708 * _en_ratio; // J/m^2
  surf_energy *= JtoeV * _length_scale*_length_scale; // Convert to eV/length_scale^2

  Real conv_int_width = _int_width;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K

    _kT[_qp] = _kb[_qp] * _T[_qp]; // _kT is in units of eV

    _Efv[_qp] = _Efvin; // eV
    _Efi[_qp] = 3.066;  // eV

    _cv_eq[_qp] = std::exp(-_Efv[_qp] / _kT[_qp]); // Unitless
    _D_v[_qp] = Dv0 * std::exp(-Emv / _kT[_qp]); // In units of length_scale^2/time_scale
    _Qstar[_qp] = -4.0; // eV

    const Real Di = Di0 * std::exp(-Emi/_kT[_qp]); // Units of length_scale^2/time_scale
    const Real Mlog = 1.092499; // Constant value calculated for log function
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

    _kappa_v[_qp] = std::sqrt(2.0) * surf_energy * conv_int_width / M;
    _W[_qp] = surf_energy / (conv_int_width * M * std::sqrt(2.0));

    if (_free_energy_form == 0) // Natural log free energy
    {
      Real cv = _cv[_qp];
      Real tol = 1.0e-9;

      if (cv < tol)
        cv = tol;
      if (cv > 1.0 - tol)
        cv = 1.0 - tol;

      _M_v[_qp] = _D_v[_qp] * (cv - cv*cv) / _kT[_qp];
      _grad_M_v[_qp] = _D_v[_qp] * _grad_cv[_qp] * (1.0 - 2.0 * cv) / _kT[_qp];
      _Ms_v[_qp] = _D_v[_qp] / _kT[_qp];
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
      _grad_M_v[_qp] = 0.0;
    }

    _M_i[_qp] = Di / _kT[_qp];
    _grad_M_i[_qp] = 0;
  }
}
