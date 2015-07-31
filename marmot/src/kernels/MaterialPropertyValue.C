#include "MaterialPropertyValue.h"

template<>
InputParameters validParams<MaterialPropertyValue>()
{
  InputParameters params = validParams<KernelValue>();
  params.addRequiredParam<MaterialPropertyName>("prop_name", "Name of material property to be used in the kernel");
  params.addParam<bool>("positive", true, "If the kernel is positive, this is true, if negative, it is false");
  return params;
}

MaterialPropertyValue::MaterialPropertyValue(const InputParameters & parameters) :
    KernelValue(parameters),
    _kernel_sign(getParam<bool>("positive") ? 1.0 : -1.0),
    _prop(getMaterialProperty<Real>("prop_name"))
{
}

Real
MaterialPropertyValue::precomputeQpResidual()
{
  return _kernel_sign * (_prop[_qp] - _u[_qp]);
}

Real
MaterialPropertyValue::precomputeQpJacobian()
{
  return - _kernel_sign * _phi[_j][_qp];
}
