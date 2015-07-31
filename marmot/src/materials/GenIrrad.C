#include "GenIrrad.h"

template<>
InputParameters validParams<GenIrrad>()
{
  InputParameters params = validParams<Material>();
  params.addParam<Real>("kappa_eta", 1.0, "The kappa parameter for the order parameter");
  params.addParam<Real>("kappa_cv", 1.0, "The kappa parameter for the vacancy concentration");
  params.addParam<Real>("kappa_ci", 1.0, "The kappa parameter for the interstitial concentration");
  params.addParam<Real>("T", 300, "The temperature in Kelvin");
  params.addParam<Real>("Diff", 1.0, "The reference diffusivity");
  params.addParam<Real>("c_eq", 1.0e-4, "The equilibrium concentration");
  MooseEnum form_enum("PHYSICAL=0 4THORDER=4 6THORDER=6 8THORDER=8", "PHYSICAL");
  params.addParam<MooseEnum>("free_energy_form", form_enum, "Free energy form. Physical or polynomial approximations.");
  params.addParam<Real>("w", 1.0, "Void parameter");
  params.addParam<Real>("a", 0.5, "x-position of energy barrier");
  params.addRequiredCoupledVar("cv", "Vacancy Concentration");
  params.addRequiredCoupledVar("ci", "Interstitial Concentration");
  return params;
}

GenIrrad::GenIrrad(const InputParameters & parameters) :
    Material(parameters),
    _cv(coupledValue("cv")),
    _grad_cv(coupledGradient("cv")),
    _ci(coupledValue("ci")),
    _grad_ci(coupledGradient("ci")),
    _kT(declareProperty<Real>("kT")),
    _M_v(declareProperty<Real>("M_v")),
    _grad_M_v(declareProperty<RealGradient>("grad_M_v")),
    _M_i(declareProperty<Real>("M_i")),
    _grad_M_i(declareProperty<RealGradient>("grad_M_i")),
    _Efv(declareProperty<Real>("Efv")),
    _Efi(declareProperty<Real>("Efi")),
    _L(declareProperty<Real>("L")),
    _cv_eq(declareProperty<Real>("cv_eq")),
    _kappa_op(declareProperty<Real>("kappa_op")),
    _kappa_v(declareProperty<Real>("kappa_v")),
    _kappa_i(declareProperty<Real>("kappa_i")),
    _kappa_eta(getParam<Real>("kappa_eta")),
    _kappa_cv(getParam<Real>("kappa_cv")),
    _kappa_ci(getParam<Real>("kappa_ci")),
    _T(getParam<Real>("T")),
    _Diff(getParam<Real>("Diff")),
    _w(getParam<Real>("w")),
    _a(getParam<Real>("a")),
    _free_energy_form(getParam<MooseEnum>("free_energy_form")),
    _kb(8.6173324e-5)
{
}

void
GenIrrad::computeProperties()
{
  const Real tol = 1.0e-6;

  for (unsigned int qp = 0; qp < _qrule->n_points(); ++qp)
  {
    _kT[qp] = _kb * _T;
    _Efv[qp] = 1.0;
    _Efi[qp] = 0.1;
    _L[qp] = 2.0;
    _cv_eq[qp] = std::exp(-_w/_kT[qp]);
    _kappa_op[qp] = _kappa_eta;
    _kappa_v[qp] = _kappa_cv;
    _kappa_i[qp] = _kappa_ci;

    if (_free_energy_form == 0)
    {
      Real cv = _cv[qp];
      RealGradient grad_cv = _grad_cv[qp];

      if (cv < tol)
        cv = tol;
      if (cv > 1.0 - tol)
        cv = 1.0 - tol;

      _M_v[qp] = _Diff * cv / _kT[qp];
      _grad_M_v[qp] = _Diff * _grad_cv[qp] / _kT[qp];
    }
    else if (_free_energy_form == 4) // 4th order polynomial
    {
      Real W = _kT[qp] * std::log(0.5) + 0.25 * _w;
      _M_v[qp] = _Diff / (32.0 * W);
      _grad_M_v[qp] = 0.0;
    }
    else if (_free_energy_form == 6) // 6th order polynomial
    {
      Real W = _kT[qp] * std::log(0.5) + 0.25 * _w;
      _M_v[qp] = _Diff / (768.0 * W * 2.0 * _a*_a*_a*_a);
      _grad_M_v[qp] = 0.0;
    }
    else if (_free_energy_form == 8) // 8th order polynomial
    {
      Real W = _kT[qp] * std::log(0.5) + 0.25 * _w;
      _M_v[qp] = _Diff / (192.0 * W);
      _grad_M_v[qp] = 0.0;
    }

    _M_i[qp] = 10.0 * _ci[qp];
    _grad_M_i[qp] = 10.0 * _grad_ci[qp];
  }
}
