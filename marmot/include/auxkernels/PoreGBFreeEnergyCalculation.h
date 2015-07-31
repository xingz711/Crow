#ifndef POREGBFREEENERGYCALCULATION_H
#define POREGBFREEENERGYCALCULATION_H

#include "AuxKernel.h"

//Forward Declarations
class PoreGBFreeEnergyCalculation;

template<>
InputParameters validParams<PoreGBFreeEnergyCalculation>();

/**
 * Free energy contribution of grain boundary pores.
 * \see PoreGBPurdueFreeEnergyCalc
 * \todo Check
 */
class PoreGBFreeEnergyCalculation : public AuxKernel
{
public:
  PoreGBFreeEnergyCalculation(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  unsigned int _ncrys;
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  VariableValue & _c;
  VariableGradient & _grad_c;

  const MaterialProperty<Real> & _kappa_c;
  const MaterialProperty<Real> & _kappa_GB;
  const MaterialProperty<Real> & _mu;
  const MaterialProperty<Real> & _gamma;

  Real _en_ratio;
};

#endif //POREGBFREEENERGYCALCULATION_H
