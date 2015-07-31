#include "RadSourceVExp.h"
#include "MooseRandom.h"

template<>
InputParameters validParams<RadSourceVExp>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<unsigned int>("exp", 1.0, "Random numbers are taken to this exponent, to make them more spikey");
  params.addRequiredCoupledVar("c", "Concentration defining the gas bubbles");
  params.addParam<Real>("lc", 0.545e-9, "Lattice constant, defaults to UO2 from govers et al (2007)");
  params.addParam<Real>("fractional_yield", 0.3017, "fraction yield of fission gas atoms per fission/m^3");
  params.addParam<Real>("fission_rate_mult", 1.0, "Fudge factor multiplied by the fission rate");
  params.addCoupledVar("fission_rate", "Coupled Fission Rate");
  return params;
}
RadSourceVExp::RadSourceVExp(const InputParameters & parameters) :
    Kernel(parameters),
    _c_old(coupledValueOld("c")),
    _exp(getParam<unsigned int>("exp")),
    _fractional_yield(getParam<Real>("fractional_yield")),
    _fission_rate_mult(getParam<Real>("fission_rate_mult")),
    _lc(getParam<Real>("lc")),
    _fission_rate(coupledValue("fission_rate")),
    _fission_rate_old(coupledValueOld("fission_rate"))
{
  Real atom_density = 4.0 / (_lc*_lc*_lc);

  // concentration increase/m^3 per fission
  _concentration_yield = _fractional_yield / atom_density;
}

void
RadSourceVExp::residualSetup()
{
  unsigned int rseed = _t_step;
  MooseRandom::seed(rseed);
}

Real
RadSourceVExp::computeQpResidual()
{
  Real Pv = 0.0;
  Real gas_source_rate =   _concentration_yield * _fission_rate_mult
                         * (_fission_rate[_qp] + _fission_rate_old[_qp]) / 2.0;

  _max = (_exp + 1) * gas_source_rate;

  if ( _c_old[_qp] < 0.8 )
  {
    Real R1 = MooseRandom::rand();
    Pv = _max * std::pow(R1, std::floor(_exp));
  }

  return -_test[_i][_qp] * Pv;
}
