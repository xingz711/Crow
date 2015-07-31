#ifndef SPLITCHWRESTGRAD_H
#define SPLITCHWRESTGRAD_H

#include "Kernel.h"

//Forward Declarations
class SplitCHWResTGrad;

template<>
InputParameters validParams<SplitCHWResTGrad>();

/**
 * This kernel computes the residual of the substitutional variable in the split cahn-hilliard formulation.
 * In addition, it includes the effects of GB and surface diffusion, if desired.
 */
class SplitCHWResTGrad : public Kernel
{
public:
  SplitCHWResTGrad(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::vector<VariableValue *> _vals;
  Real _diffindex;
  Real _gbindex;
  Real _bulkindex;
  unsigned int _tmpt_var;
  VariableValue & _tmpt;
  VariableGradient & _grad_tmpt;
  unsigned int _c_var;
  VariableValue & _c;
  const MaterialProperty<Real> & _D_v;
  const MaterialProperty<Real> & _Qheat;

  unsigned int _ncrys;

  const Real _kb;
};

#endif //SPLITCHWRESTGRAD_H
