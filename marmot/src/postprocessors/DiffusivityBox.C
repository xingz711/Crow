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

#include "DiffusivityBox.h"

#include <limits>

template<>
InputParameters validParams<DiffusivityBox>()
{
  InputParameters params = validParams<NodalVariablePostprocessor>();
  params.addParam<Real>("length", 40, "The length of the simulation cell");
  params.addParam<Real>("flux", 1e-12, "The species flux");
  params.addRequiredCoupledVar("c3", "XeVa concentration");
  return params;
}

DiffusivityBox::DiffusivityBox(const InputParameters & parameters) :
    NodalVariablePostprocessor(parameters),
    _length(parameters.get<Real>("length")),
    _flux(parameters.get<Real>("flux")),
    _c3(coupledValue("c3"))
{
}

void
DiffusivityBox::initialize()
{
  _value_min = std::numeric_limits<Real>::max(); // start w/ the min
  _value_max = -std::numeric_limits<Real>::max(); // start w/ the max
}

void
DiffusivityBox::execute()
{
  _value_min = std::min(_value_min, _u[_qp] + _c3[_qp]);
  _value_max = std::max(_value_max, _u[_qp] + _c3[_qp]);
}

Real
DiffusivityBox::getValue()
{
  gatherMax(_value_max);
  gatherMin(_value_min);

  return _flux/(_value_max-_value_min)*_length;
}

void
DiffusivityBox::threadJoin(const UserObject & y)
{
  const DiffusivityBox & pps = static_cast<const DiffusivityBox &>(y);

  _value_max = std::max(_value_max, pps._value_max);
  _value_min = std::min(_value_min, pps._value_min);
}
