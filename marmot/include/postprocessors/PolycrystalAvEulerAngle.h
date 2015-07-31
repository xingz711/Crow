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

#ifndef POLYCRYSTALAVEULERANGLE_H
#define POLYCRYSTALAVEULERANGLE_H

#include "ElementIntegralPostprocessor.h"

//Forward Declarations
class PolycrystalAvEulerAngle;

template<>
InputParameters validParams<PolycrystalAvEulerAngle>();

/**
 * Compute the average euler angle in a polycrystal.
 */
class PolycrystalAvEulerAngle : public ElementIntegralPostprocessor
{
public:
  PolycrystalAvEulerAngle(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

protected:
  virtual Real computeQpIntegral();

private:
  unsigned int _angle_number;
  unsigned int _grain_number;
  const MaterialProperty<Real> & _EulerAngle1;
  const MaterialProperty<Real> & _EulerAngle2;
  const MaterialProperty<Real> & _EulerAngle3;
  bool _numerator;
  Real _integral_value_numerator, _integral_value_denominator;
  Real _angle;
  unsigned int _current_var;

  const unsigned int _nvals;
  std::vector<VariableValue *> _vals;
};

#endif //POLYCRYSTALAVEULERANGLE_H
