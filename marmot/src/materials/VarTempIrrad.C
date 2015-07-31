#include "VarTempIrrad.h"

template<>
InputParameters validParams<VarTempIrrad>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temp", "Temperature");
  params.addRequiredCoupledVar("c", "Concentration");
  params.addParam<Real>("kappa_cv_input", 1.0, "The kappa parameter for the vacancy concentration");

  return params;
}

VarTempIrrad::VarTempIrrad(const InputParameters & parameters) :
    Material(parameters),
    _temp(coupledValue("temp")),
    _grad_temp(coupledGradient("temp")),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _M_v(declareProperty<Real>("M_v")),
    _grad_M_v(declareProperty<RealGradient>("grad_M_v")),
    _M_i(declareProperty<Real>("M_i")),
    _Efv(declareProperty<Real>("Efv")),
    _Efi(declareProperty<Real>("Efi")),
    _Emv(declareProperty<Real>("Emv")),
    _Emi(declareProperty<Real>("Emi")),
    _kT(declareProperty<Real>("kT")),
    _L_eta(declareProperty<Real>("L_eta")),
    _kb(declareProperty<Real>("kb")),
    _kappa_cv(declareProperty<Real>("kappa_cv")),
    _kappa_cv_input(getParam<Real>("kappa_cv_input"))
{
}

void
VarTempIrrad::computeProperties()
{
  const Real Dv0 = 1.3746e5;
  const Real Di0 = 1.3746e6;

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
  {
    _kb[_qp] = 8.617343e-5; // Boltzmann constant in eV/K
    _kT[_qp] = _kb[_qp]*_temp[_qp];

    _Efv[_qp] = 1.0; // eV
    _Efi[_qp] = 1.0; // eV

    _Emv[_qp] = 1.0; // eV
    _Emi[_qp] = 1.0; // eV

    Real Dv = Dv0 * std::exp(-_Emv[_qp]/_kT[_qp]);
    _M_v[_qp] = _c[_qp] * Dv;
    RealGradient grad_Dv = Dv * _Emv[_qp] / (_kT[_qp]*_temp[_qp]) * _grad_temp[_qp];
    _grad_M_v[_qp] = _grad_c[_qp] * Dv + _c[_qp] * grad_Dv;

    Real Di = Di0 * std::exp(-_Emi[_qp]/_kT[_qp]);
    _M_i[_qp] = Di;

    _L_eta[_qp] = 1.0;

    _kappa_cv[_qp]=_kappa_cv_input;
  }
}
