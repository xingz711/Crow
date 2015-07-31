/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "FissionGasCoupledLinear.h"

template<>
InputParameters validParams<FissionGasCoupledLinear>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("v", "The coupled variable which provides the linear contribution");
  params.addRequiredParam<std::string>("a", "The material property that is multiplied onto the variable");
  params.addParam<Real>("b", 1.0, "Constant prefactor for the residual term");
  return params;
}

FissionGasCoupledLinear::FissionGasCoupledLinear(const InputParameters & parameters) :
    Kernel(parameters),
    _v_var(coupled("v")),
    _v(coupledValue("v")),
    _a_name(getParam<std::string>("a")),
    _a(getMaterialProperty<Real>(_a_name)),
    _b(getParam<Real>("b"))
{
}

Real
FissionGasCoupledLinear::computeQpResidual()
{
  return _a[_qp] * _b * _v[_qp] * _test[_i][_qp];
}

Real
FissionGasCoupledLinear::computeQpJacobian()
{
  return 0.0;
}

Real
FissionGasCoupledLinear::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _v_var)
    return _a[_qp] * _b * _phi[_j][_qp] * _test[_i][_qp];

  return 0.0;
}
