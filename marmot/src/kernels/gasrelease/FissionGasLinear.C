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

#include "FissionGasLinear.h"

template<>
InputParameters validParams<FissionGasLinear>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<std::string>("a", "The material property that is multiplied onto the variable");
  params.addParam<Real>("b", 1.0, "Constant prefactor for the residual term");
  return params;
}

FissionGasLinear::FissionGasLinear(const InputParameters & parameters) :
    Kernel(parameters),
    _a_name(getParam<std::string>("a")),
    _a(getMaterialProperty<Real>(_a_name)),
    _b(getParam<Real>("b"))
{
}

Real
FissionGasLinear::computeQpResidual()
{
  return _a[_qp] * _b * _u[_qp] * _test[_i][_qp];
}

Real
FissionGasLinear::computeQpJacobian()
{
  return _a[_qp] * _b * _phi[_j][_qp] *_test[_i][_qp];
}
