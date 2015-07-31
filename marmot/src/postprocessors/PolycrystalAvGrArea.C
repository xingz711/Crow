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

#include "PolycrystalAvGrArea.h"

template<>
InputParameters validParams<PolycrystalAvGrArea>()
{
  InputParameters params = validParams<ElementIntegralVariablePostprocessor>();
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled variables");
  return params;
}

PolycrystalAvGrArea::PolycrystalAvGrArea(const InputParameters & parameters) :
    ElementIntegralVariablePostprocessor(parameters),
    _integral_value(0),
    _nvals(coupledComponents("v"))
    // _op_num(getParam<int>("op_num")),
    // _var_name_base(getParam<std::string>("var_name_base"))
{
  _vals.resize(_nvals);
  for (unsigned int i = 0; i < _nvals; ++i)
  {
    /*Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << i;
    var_name.append(out.str());*/

    _vals[i] = &coupledValue("v", i);
    // _vals[i] = &coupledValue(var_name);
  }

  _integral_value.resize(_nvals);
}

void
PolycrystalAvGrArea::initialize()
{
  for (unsigned int i = 0; i < _nvals; ++i)
    _integral_value[i] = 0;
}

void
PolycrystalAvGrArea::execute()
{
  for (unsigned int i = 0; i < _nvals; ++i)
  {
    _current_var = i;
    _integral_value[i] += computeIntegral();
  }
}

Real
PolycrystalAvGrArea::getValue()
{
  Real ave_val = 0.0;
  unsigned int num_grains = 0.0;

  for (unsigned int i = 0; i < _nvals; ++i)
  {
    // Calculate total grain area
    gatherSum(_integral_value[i]);
    ave_val += _integral_value[i];
    // Determie number of grains
    if (_integral_value[i] > 100.0)
      num_grains++;
  }

  // Calculate average grain area
  return ave_val / num_grains;
}

void
PolycrystalAvGrArea::threadJoin(const UserObject & y)
{
  const PolycrystalAvGrArea & pps = dynamic_cast<const PolycrystalAvGrArea &>(y);
  for (unsigned int i=0; i<_nvals; ++i)
    _integral_value[i] += pps._integral_value[i];
}

Real
PolycrystalAvGrArea::computeQpIntegral()
{
  Real val = (*_vals[_current_var])[_qp];
  if (val < 1.0e-5)
    val = 0.0;

  return (*_vals[_current_var])[_qp];
}
