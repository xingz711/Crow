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

#ifndef COMPUTEGRAINCENTERUSEROBJECT_H
#define COMPUTEGRAINCENTERUSEROBJECT_H

#include "ElementPostprocessor.h"

//Forward Declarations
class ComputeGrainCenterUserObject;

template<>
InputParameters validParams<ComputeGrainCenterUserObject>();

/**
 * This postprocessor computes a volume integral of the specified variable.
 *
 * Note that specializations of this integral are possible by deriving from this
 * class and overriding computeQpIntegral().
 */
class ComputeGrainCenterUserObject : public ElementUserObject
{
public:
  ComputeGrainCenterUserObject(const std::string & name, InputParameters parameters);

  virtual void initialize();
  virtual void execute();
  virtual void threadJoin(const UserObject & y);
  virtual Real getValue();
  virtual void finalize();

protected:
  virtual Real computeQpIntegral();
  virtual Real computeIntegral();

  unsigned int _qp;
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  std::vector<unsigned int> _vals_var;
  std::vector<Real> _grain_volume;

  unsigned int _nargs;
};

#endif //COMPUTEGRAINCENTERUSEROBJECT_H
