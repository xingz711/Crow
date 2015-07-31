#ifndef CHInterface4_H
#define CHInterface4_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class CHInterface4;

template<>
InputParameters validParams<CHInterface4>();
/**
 * This is the Cahn-Hilliard equation base class that implements the interfacial or gradient energy term of the equation.
 * See M.R. Tonks et al. / Computational Materials Science 51 (2012) 20–29 for more information.
 */

class CHInterface4 : public Kernel
{
public:

  CHInterface4(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _M;

  const MaterialProperty<RealGradient> & _grad_M;

  VariableSecond & _second_u;
  VariableTestSecond & _second_test;
  VariablePhiSecond & _second_phi;
};
#endif //CHInterface_H
