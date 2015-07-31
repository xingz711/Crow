/*
Formulation: Miehe et. al., Int. J. Num. Methods Engg., 2010, 83. 1273-1311
Equation 63
*/

#ifndef ACFracCoupledInterface_H
#define ACFracCoupledInterface_H

#include "KernelGrad.h"

//Forward Declarations
class ACFracCoupledInterface;

template<>
InputParameters validParams<ACFracCoupledInterface>();

class ACFracCoupledInterface : public KernelGrad
{
public:
  ACFracCoupledInterface(const InputParameters & parameters);

protected:
  virtual RealGradient precomputeQpResidual();
  virtual RealGradient precomputeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  VariableValue & _c;
  VariableGradient & _grad_c;
  unsigned int _c_var;

private:
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _L;
};

#endif //ACInterface_H
