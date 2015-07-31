#ifndef CHVACGB_H
#define CHVACGB_H

#include "CHBulk.h"

//Forward Declarations
class CHVacGB;

template<>
InputParameters validParams<CHVacGB>();

/**
 * CHVacGB calculates the necessary values for the residual and jacobian of the vacancy/gb interaction model proposed by Paul Millett.
 * It must be run with CHInterface to define the entire residual
 */
class CHVacGB : public CHBulk
{
public:
  CHVacGB(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  const MaterialProperty<Real> & _Ao;
  const MaterialProperty<Real> & _BetaS;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
};

#endif //CHVACGB_H
