#include "FeCrFreeEnergyCalculation.h"

template<>

InputParameters validParams<FeCrFreeEnergyCalculation>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("c", "Concentration of Cr" );
  params.addRequiredCoupledVar("Temperature", "Temperature the systems resides at" );
  params.addRequiredParam<Real>("kappa","Penalty interface parameter");
  return params;
}

FeCrFreeEnergyCalculation::FeCrFreeEnergyCalculation(const InputParameters & parameters) :
   AuxKernel(parameters),
    _c(coupledValue("c")),
    _grad_c(coupledGradient("c")),
    _T(coupledValue("Temperature")),
    _kappa_value(getParam<Real>("kappa"))
{
}

Real
FeCrFreeEnergyCalculation::computeValue()
{
  Real chi = _c[_qp]; // this is the concentration variable: [Cr] (Wt. percentage) in alloy
  Real T0 = 410; // Switching temperature-->Schwen used this in Mathematica file
  Real T = _T[_qp]; // T is the second variable
  Real K_B = 8.6173324e-5; // Boltzmann constant
  Real t= 1 - 2 * chi;
  Real p= 1 - chi;

  // values of constants taken from Schwen2013 (f,a-->d)

  // first to last element within array represents f0-->f5
  const Real f[] = {0.3817, -1.007, -0.0485, 0.1541, -0.1684, -0.0416, -3.8602, -4.1671};
  // first to last element represents a0-->a7
  const Real a[] = {0.3856, -0.0973, -0.0467, -0.1945, -0.1856, -0.0044, -3.8366, -4.1231};
  // first to last element represents b0-->b3
  const Real b[] = {0.0003, 4.6956e-5, -4.9588e-5, 1.1333e-5};
  // first to last element represents c0-->c3
  const Real c[] = {3.1446e-9, -2.2031e-8, 3.9597e-8, -3.0902e-8};
  // first to last element represents d0-->d3
  const Real d[] = {-1.7601e-13, 5.5788e-12, -5.9353e-12, 1.2285e-11};

  Real G0 = chi * p * (f[0] + f[1] * t + f[2] * std::pow(t, 2.0) + f[3] * std::pow(t, 3.0) + f[4] * std::pow(t, 4.0) + f[5] * std::pow(t, 5.0)) + f[6] * chi + f[7] * p;
  Real term1 = G0 * (T / T0);

  Real Ha = chi * p * (a[0] + a[1] * t + a[2] * std::pow(t, 2.0) + a[3] * std::pow(t, 3.0) + a[4] * std::pow(t, 4.0) + a[5] * std::pow(t, 5.0)) + a[6] * chi + a[7] * p;
  Real term2 = Ha;

  Real Hb = b[0] + b[1] * p + b[2] * std::pow(p, 2.0) + b[3] * std::pow(p, 3.0);
  Real term3 = -1 * Hb * T * std::log(T);

  Real Hc = c[0] + c[1] * p + c[2] * std::pow(p, 2.0) + c[3] * std::pow(p, 3.0);
  Real term4 = -1.0 * Hc * (std::pow(T, 2.0));

  Real Hd = d[0] + d[1] * p + d[2] * std::pow(p, 2.0) + d[3] * std::pow(p, 3.0);
  Real term5 = 0.5 * Hd * (std::pow(T, 3.0));

  Real He = (-Ha / T0) + (Hc * T0) + (Hb * std::log(T0)) + (Hd * 0.5 * std::pow(T0, 2.0));
  Real term6 = T * He;

  Real term7 = K_B * T * (chi * std::log(chi) + (1.0 - chi) * std::log(1.0 - chi));

  Real G_bulk = term1 + term2 + term3 + term4 + term5 + term6 + term7;

  // Interfacial energies
  Real G_int = 0.5 * _kappa_value * _grad_c[_qp] * _grad_c[_qp];

  // Total Free energy
  return G_bulk + G_int;
}
