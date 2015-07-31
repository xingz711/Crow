#ifndef FECRFREEENERGYCALCULATION_H
#define FECRFREEENERGYCALCULATION_H

#include "AuxKernel.h"

//Forward Declarations
class FeCrFreeEnergyCalculation;

template<>
InputParameters validParams<FeCrFreeEnergyCalculation>();

/**
 * Free energy of FeCr system Schwen2013
 */
class FeCrFreeEnergyCalculation : public AuxKernel
{
public:
  FeCrFreeEnergyCalculation(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _c;
  VariableGradient & _grad_c;
  VariableValue & _T;

  Real _kappa_value;
};

#endif //FECRFREEENERGYCALCULATION_H
