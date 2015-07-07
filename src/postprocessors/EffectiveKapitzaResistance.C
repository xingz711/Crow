#include "EffectiveKapitzaResistance.h"

template<>
InputParameters validParams<EffectiveKapitzaResistance>()
{
  InputParameters params = validParams<GeneralPostprocessor>();
  params.addRequiredParam<Real>("dx", "Length between sides of sample");
  params.addRequiredParam<Real>("ko", "Conductivity of bulk portion of bicrystal");
  params.addParam<Real>("length_scale", 1e-8, "Lenthscale of the problem, in meters, default is 1e-8");
  params.addRequiredParam<PostprocessorName>("eff_cond", "The effective conductivity across the GB");
  return params;
}

EffectiveKapitzaResistance::EffectiveKapitzaResistance(const std::string & name,
                                                       InputParameters parameters) :
    GeneralPostprocessor(name, parameters),
    _dx(getParam<Real>("dx")),
    _ko(getParam<Real>("ko")),
    _length_scale(getParam<Real>("length_scale")),
    _eff_cond(getPostprocessorValue("eff_cond"))
{
}

Real
EffectiveKapitzaResistance::getValue()
{
  Real k = _eff_cond;
  return (_dx * _length_scale) * (_ko - k) / (_ko * k);
}
