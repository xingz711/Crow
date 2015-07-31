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

#include "PolycrystalAvEulerAngle.h"

template<>
InputParameters validParams<PolycrystalAvEulerAngle>()
{
  InputParameters params = validParams<ElementIntegralPostprocessor>();
  params.addClassDescription("Compute the average Euler angle in a polycrystal");
  params.addRequiredCoupledVar("v", "Array of coupled variables");
  params.addRequiredParam<unsigned int>("angle_number", "Euler angle number");
  params.addRequiredParam<unsigned int>("grain_number", "Grain number");
  return params;
}

PolycrystalAvEulerAngle::PolycrystalAvEulerAngle(const InputParameters & parameters) :
    ElementIntegralPostprocessor(parameters),
    _angle_number(getParam<unsigned int>("angle_number")),
    _grain_number(getParam<unsigned int>("grain_number")),
    _EulerAngle1(getMaterialProperty<Real>("angle1")),
    _EulerAngle2(getMaterialProperty<Real>("angle2")),
    _EulerAngle3(getMaterialProperty<Real>("angle3")),
    _numerator(false),
    _integral_value_numerator(0),
    _integral_value_denominator(0),
    _nvals(coupledComponents("v"))
{
  _vals.resize(_nvals);
  for (unsigned int i=0; i<_vals.size(); ++i)
    _vals[i] = &coupledValue("v", i);
}

void
PolycrystalAvEulerAngle::initialize()
{
  _integral_value_numerator = 0;
  _integral_value_denominator = 0;
}

void
PolycrystalAvEulerAngle::execute()
{
  _current_var = _grain_number;

  _numerator = true;
  _integral_value_numerator += computeIntegral();
  _numerator = false;

  _integral_value_denominator += computeIntegral();
}

Real
PolycrystalAvEulerAngle::getValue()
{
  gatherSum(_integral_value_numerator);
  gatherSum(_integral_value_denominator);

  if (_integral_value_denominator <= 0.0)
    return 0.0;
  else
    return _integral_value_numerator / _integral_value_denominator;
}

void
PolycrystalAvEulerAngle::threadJoin(const UserObject & y)
{
  const PolycrystalAvEulerAngle & pps = dynamic_cast<const PolycrystalAvEulerAngle &>(y);

  _integral_value_numerator += pps._integral_value_numerator;
  _integral_value_denominator += pps._integral_value_denominator;
}

Real
PolycrystalAvEulerAngle::computeQpIntegral()
{
  // std::ofstream myfile_test("check_test.txt", std::ios::app);

  if (_numerator)
  {
    if (_angle_number == 1)
      _angle = _EulerAngle1[_qp];

    if (_angle_number == 2)
      _angle = _EulerAngle2[_qp];

    if (_angle_number == 3)
      _angle = _EulerAngle3[_qp];

    // Real val = (*_vals[_current_var])[_qp];
    // if (val < 1.0e-10)
    //  val = 1.0e-10;

    // return ((*_vals[_current_var])[_qp] * _angle);

    Real max_val, val;
    unsigned int index(0);

    max_val = 0.0;

    for (unsigned int i = 0; i < _nvals; ++i)
    {
      val = (*_vals[i])[_qp];
      if (val > max_val)
      {
        max_val = val;
        index = i;
      }
    }

    if (index == _current_var)
      return _angle;
    else
      return 0.0;
  }
  else
  {
    // Real val = (*_vals[_current_var])[_qp];
    // if (val < 1.0e-10)
    //   val = 1.0e-10;

    // return (*_vals[_current_var])[_qp];

    Real max_val, val;
    unsigned int index(0);

    max_val = 0.0;
    for (unsigned int i = 0; i < _nvals; ++i)
    {
      val = (*_vals[i])[_qp];
      if (val > max_val)
      {
        max_val = val;
        index = i;
      }
    }

    if (index == _current_var)
      return 1.0;
    else
      return 0.0;
  }
}
