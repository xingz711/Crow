#include "PoreGBFreeEnergyCalculation.h"

template<>
InputParameters validParams<PoreGBFreeEnergyCalculation>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredCoupledVar("c", "Concentration defining pores" );
  params.addParam<Real>("en_ratio", 1.0, "ratio of surface energy to GB energy");

  return params;
}

PoreGBFreeEnergyCalculation::PoreGBFreeEnergyCalculation(const InputParameters & parameters) :
    AuxKernel(parameters),
    _ncrys(coupledComponents("v")),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _kappa_c(getMaterialProperty<Real>("kappa_c")),
    _kappa_GB(getMaterialProperty<Real>("kappa_op")),
    _mu(getMaterialProperty<Real>("mu")),
    _gamma(getMaterialProperty<Real>("gamma_asymm")),
    _en_ratio(getParam<Real>("en_ratio"))
{
  _vals.resize(_ncrys);
  _grad_vals.resize(_ncrys);

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _grad_vals[i] = &coupledGradient("v", i);
  }
}

Real
PoreGBFreeEnergyCalculation::computeValue()
{
  Real eta_i2_eta_j2 = 0;

  for (unsigned int i = 0; i < _ncrys; ++i)
    for (unsigned int j = i+1; j < _ncrys; ++j)
        eta_i2_eta_j2 += (*_vals[i])[_qp]*(*_vals[i])[_qp]*(*_vals[j])[_qp]*(*_vals[j])[_qp];

  Real eta4meta2 = 0;
  Real c2eta2 = 0;

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    eta4meta2 += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp] / 4.0 - (*_vals[i])[_qp] * (*_vals[i])[_qp] / 2.0;
    c2eta2 +=  _c[_qp]* _c[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp];
  }

  Real c4mc2 = _c[_qp] * _c[_qp] * _c[_qp] * _c[_qp] / 4.0 -  _c[_qp] * _c[_qp] / 2.0;

  // Interfacial energies
  Real int_energy = _kappa_c[_qp]/2.0 * _grad_c[_qp].size() * _grad_c[_qp].size();

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    Real abs_grad_eta = ((*_grad_vals[i])[_qp].size());
    int_energy += _kappa_GB[_qp]/2.0 * abs_grad_eta * abs_grad_eta;
  }

  // Total bulk energy
  Real bulk_energy =   eta4meta2 + c4mc2 + _gamma[_qp]*eta_i2_eta_j2
                     + _gamma[_qp] * _en_ratio * c2eta2 + 1.0/4.0;
  // Real bulk_energy =  _gamma[_qp]*eta_i2_eta_j2 ;

  // Total energy
  Real value = _mu[_qp] * bulk_energy + int_energy;

  return value;
}
