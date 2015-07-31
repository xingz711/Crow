#ifndef SPLITCHTWOCOMP_H
#define SPLITCHTWOCOMP_H

#include "SplitCHCRes.h"

//Forward Declarations
class SplitCHTwoComp;

template<>
InputParameters validParams<SplitCHTwoComp>();

/**
 * The couple, SplitCHTwoComp and SplitCHWRes, splits the CH equation by replacing chemical potential with 'w'.
 * See "Hybrid Potts-phase field model for coupled microstructural-compositional evolution".
 * E. R. Homer, V. Tikare, E. A. Holm
 * Computational Material Science, 69, p. 414-423.
 */
class SplitCHTwoComp : public SplitCHCRes
{
public:
  SplitCHTwoComp(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

  const Real _a;
  const Real _lambda;
  const Real _c1;
  const Real _c2;
  const Real _c3;
  const Real _c4;
  const VariableValue & _q_alpha;
};

#endif //SPLITCHTWOCOMP_H
