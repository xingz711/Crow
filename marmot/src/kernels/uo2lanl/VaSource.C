#include "VaSource.h"

template<>
InputParameters validParams<VaSource>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<Real>("R", "Rate of production Concentration");
  params.addRequiredParam<Real>("Factor", "Interstitial bias");
  return params;
}

VaSource::VaSource(const InputParameters & parameters) :
    Kernel(parameters),
    _R(getParam<Real>("R")),
    _F(getParam<Real>("Factor"))
{
}

Real
VaSource::computeQpResidual()
{
  //Calculates defect production, specifically for Va

  return - _test[_i][_qp] * _R * _F;
}

Real
VaSource::computeQpJacobian()
{
  return 0;
}
