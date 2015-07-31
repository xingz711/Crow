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

#ifndef DIFFUSIVITYBOX_H
#define DIFFUSIVITYBOX_H

#include "NodalVariablePostprocessor.h"

//Forward Declarations
class DiffusivityBox;

// Input parameters
template<>
InputParameters validParams<DiffusivityBox>();

/**
 * Compute the nodal min or max value
 * \todo This Postprocessor has no test coverage
 */
class DiffusivityBox : public NodalVariablePostprocessor
{
public:
  DiffusivityBox(const InputParameters & parameters);

  virtual void initialize();
  virtual void execute();
  virtual Real getValue();
  virtual void threadJoin(const UserObject & y);

protected:
  Real _length;
  Real _flux;

  /// The extreme value
  Real _value_min;
  Real _value_max;

private:
  VariableValue & _c3;
};

#endif //DIFFUSIVITYBOX_H
