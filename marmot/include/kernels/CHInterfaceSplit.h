#ifndef CHINTERFACESPLIT_H
#define CHINTERFACESPLIT_H

#include "KernelGrad.h"

//Forward Declarations
class CHInterfaceSplit;

template<>
InputParameters validParams<CHInterfaceSplit>();

class CHInterfaceSplit : public KernelGrad
{
public:
  CHInterfaceSplit(const InputParameters & parameters);

protected:
  virtual RealGradient precomputeQpResidual();
  virtual RealGradient precomputeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  Real _var_w;
  VariableGradient & _grad_w;

  const MaterialProperty<Real> & _M;
  const MaterialProperty<Real> & _kappa;
};

#endif //CHINTERFACESPLIT_H
