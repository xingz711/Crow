#ifndef SPLITCHGBPOLY_H
#define SPLITCHGBPOLY_H

#include "SplitCHBase.h"

//Forward Declarations
class SplitCHGBPoly;

template<>
InputParameters validParams<SplitCHGBPoly>();

/**
 * CHGBPoly computes the interaction term between grains and c in the split Cahn-Hilliard equation
 */
class SplitCHGBPoly : public SplitCHBase
{
public:
  SplitCHGBPoly(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);

private:
  std::vector<VariableValue *> _vals;

  Real _en_ratio;

  const MaterialProperty<Real> & _mu;

  unsigned int _ncrys;
};

#endif //SPLITCHGBPOLY_H
