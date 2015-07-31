#include "UISource.h"

template<>
InputParameters validParams<UISource>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<Real>("R", "Rate of production Concentration");
  params.addRequiredParam<Real>("Factor", "Interstitial bias");
  return params;
}

UISource::UISource(const InputParameters & parameters) :
    Kernel(parameters),
    _R(getParam<Real>("R")),
    _F(getParam<Real>("Factor"))
{
}

Real
UISource::computeQpResidual()
{

  //Calculates defect production, specifically for UI

  return -_test[_i][_qp] * _R * _F;
}

Real
UISource::computeQpJacobian()
{
  return 0.0;
}
