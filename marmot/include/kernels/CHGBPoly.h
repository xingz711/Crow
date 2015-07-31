#ifndef CHGBPOLY4_H
#define CHGBPOLY4_H

#include "CHBulk.h"

//Forward Declarations
class CHGBPoly;

template<>
InputParameters validParams<CHGBPoly>();

/**
 * CHGBPoly contains a polynomial double well free energy functionals for simulating void nucleation and growth.
 * It used polynomial orders 4, 6 and 8 with zeros at cv_eq and 1.  It must be used in conjunction with
 * CHGBinterface to represent the entire Cahn-Hilliard equation.
 */
class CHGBPoly : public CHBulk
{
public:
  CHGBPoly(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  const MaterialProperty<Real> & _mu;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  Real _en_ratio;
};

#endif //CHGBPOLY_H
