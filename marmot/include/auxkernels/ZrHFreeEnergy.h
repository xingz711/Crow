#ifndef ZRH_FREE_ENERGY_H
#define ZRH_FREE_ENERGY_H

#include "AuxKernel.h"

class ZrHFreeEnergy;

template<>
InputParameters validParams<ZrHFreeEnergy>();

/**
 * Compute a simple free energy from a concentration as
 * \f$ \frac12\kappa_c(\nabla c)^2 + \frac14 (1-c^2)^2
 */
class ZrHFreeEnergy : public AuxKernel
{
public:
  ZrHFreeEnergy(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _eta1;
  VariableValue & _eta2;
  VariableValue & _eta3;
  VariableValue & _C;

  const MaterialProperty<Real> & _G;
  const Real _b;
  const Real _c;
  const Real _a;
  const Real _d;
  const Real _C_m;
  const Real _C_gamma;

};

#endif //ZRH_FREE_ENERGY_H
