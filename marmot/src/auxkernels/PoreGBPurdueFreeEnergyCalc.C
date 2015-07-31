#include "PoreGBPurdueFreeEnergyCalc.h"

template<>
InputParameters validParams<PoreGBPurdueFreeEnergyCalc>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  params.addRequiredCoupledVar("c", "Concentration defining pores" );

  return params;
}

PoreGBPurdueFreeEnergyCalc::PoreGBPurdueFreeEnergyCalc(const InputParameters & parameters) :
    AuxKernel(parameters),
    _ncrys(coupledComponents("v")),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _kappa_c(getMaterialProperty<Real>("kappa_c")),
    _kappa_GB(getMaterialProperty<Real>("kappa_op")),
    _B(getMaterialProperty<Real>("B")),
    _C(getMaterialProperty<Real>("C"))
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
PoreGBPurdueFreeEnergyCalc::computeValue()
{
  Real sumeta2 = 0.0;
  Real sumeta3 = 0.0;
  Real c = _c[_qp];
  Real onemc = 1.0 - c;

  for (unsigned int i=0; i<_ncrys; ++i)
  {
    sumeta2 += (*_vals[i])[_qp] * (*_vals[i])[_qp];
    sumeta3 += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp];
  }

  // Interfacial energies
  Real int_energy = _kappa_c[_qp]/2.0 * _grad_c[_qp].size() * _grad_c[_qp].size();

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    Real abs_grad_eta = (*_grad_vals[i])[_qp].size();
    int_energy += _kappa_GB[_qp]/2.0 * abs_grad_eta * abs_grad_eta;
  }

  // Total bulk energy
  Real bulk1 = _B[_qp] * c*c * onemc*onemc;
  Real bulk2 = _C[_qp] * (onemc*onemc + 6.0 * c * sumeta2 - 4.0 * (c + 1.0) * sumeta3 + 3.0 * sumeta2*sumeta2);
  Real bulk_energy = bulk1 + bulk2;

  // Total energy
  Real value = bulk_energy + int_energy;

  return value;
}
