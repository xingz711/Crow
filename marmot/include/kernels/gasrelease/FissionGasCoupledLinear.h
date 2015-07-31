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

#ifndef FISSIONGASCOUPLEDLINEAR_H
#define FISSIONGASCOUPLEDLINEAR_H

#include "Kernel.h"

// Forward Declaration
class FissionGasCoupledLinear;

template<>
InputParameters validParams<FissionGasCoupledLinear>();

/**
 * Simple class to demonstrate off diagonal Jacobian contributions.
 */
class FissionGasCoupledLinear : public Kernel
{
public:
  FissionGasCoupledLinear(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  unsigned int _v_var;
  VariableValue & _v;

  std::string _a_name;
  const MaterialProperty<Real> & _a;

  Real _b;
};

#endif //FISSIONGASCOUPLEDLINEAR_H
