#ifndef FECRCHBULK_H
#define FECRCHBULK_H

#include "CHBulk.h"

//Forward Declarations
class FeCrCHBulk;

template<>
InputParameters validParams<FeCrCHBulk>();

/**
 * Cahn-Hilliard Kernel implementing the free energy given in Schwen et al JOM 439 180-184 -2013. df/dc has been
 * obtained by differentiating by hand
 */
class FeCrCHBulk : public CHBulk
{
public:
  FeCrCHBulk(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  VariableGradient & _grad_T;
  VariableValue & _T;
};

#endif //FECRCHBULK_H
