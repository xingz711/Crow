#ifndef RADSOURCEVEXP_H
#define RADSOURCEVEXP_H

#include "Kernel.h"

//Forward Declarations
class RadSourceVExp;

template<>
InputParameters validParams<RadSourceVExp>();

class RadSourceVExp : public Kernel
{
public:
  RadSourceVExp(const InputParameters & parameters);

protected:
  virtual void residualSetup();
  virtual Real computeQpResidual();

private:
  VariableValue & _c_old;

  Real _max;
  unsigned int _exp;
  const Real _fractional_yield;
  const Real _fission_rate_mult;
  const Real _lc;

  VariableValue  & _fission_rate;
  VariableValue  & _fission_rate_old;

  Real _concentration_yield;
};

#endif //RADSOURCEVEXP_H
