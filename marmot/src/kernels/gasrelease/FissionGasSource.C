#include "FissionGasSource.h"

template<>
InputParameters validParams<FissionGasSource>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<std::string>("a", "The material property containing the source strength");
  params.addParam<Real>("b", 1.0, "Constant prefactor for the residual term (positive for sources, negative for sinks)");
  return params;
}

FissionGasSource::FissionGasSource(const InputParameters & parameters) :
    Kernel(parameters),
    _a_name(getParam<std::string>("a")),
    _a(getMaterialProperty<Real>(_a_name)),
    _b(getParam<Real>("b"))
{
}

Real
FissionGasSource::computeQpResidual()
{
  return -_test[_i][_qp] * _a[_qp] * _b;
}

Real
FissionGasSource::computeQpJacobian()
{
  return 0.0;
}
