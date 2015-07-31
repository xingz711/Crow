#include "CalculateGasOnGB.h"
#include "Transient.h"

template<>
InputParameters validParams<CalculateGasOnGB>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<Real>("Pg", "Volumetric production rate of gas");
  params.addRequiredParam<Real>("radius", "Grain radius");
  params.addRequiredParam<PostprocessorName>("gas_in_grain", "Integrated gas in grain");

  return params;
}

CalculateGasOnGB::CalculateGasOnGB(const InputParameters & parameters) :
    GeneralPostprocessor(parameters),
    _Pg(getParam<Real>("Pg")),
    _radius(getParam<Real>("radius")),
    _gas_in_grain(getPostprocessorValue("gas_in_grain"))
{
}

Real
CalculateGasOnGB::getValue()
{
  Real Ngi = _gas_in_grain;      // integrated gas inside grain
  Real Ngt = _Pg * _radius * _t; // total gas produced

  return Ngt - Ngi;
}
