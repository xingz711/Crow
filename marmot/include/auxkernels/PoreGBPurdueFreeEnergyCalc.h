#ifndef POREGBPURDUEFREEENERGYCALC_H
#define POREGBPURDUEFREEENERGYCALC_H

#include "AuxKernel.h"

//Forward Declarations
class PoreGBPurdueFreeEnergyCalc;

template<>
InputParameters validParams<PoreGBPurdueFreeEnergyCalc>();

/**
 * Free energy contribution of grain boundary pores.
 * \see PoreGBFreeEnergyCalculation
 * \todo Check
 */
class PoreGBPurdueFreeEnergyCalc : public AuxKernel
{
public:
  PoreGBPurdueFreeEnergyCalc(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  unsigned int _ncrys;
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  VariableValue & _c;
  VariableGradient & _grad_c;

  const MaterialProperty<Real> & _kappa_c;
  const MaterialProperty<Real> & _kappa_GB;
  const MaterialProperty<Real> & _B;
  const MaterialProperty<Real> & _C;
};

#endif //POREGBPURDUEFREEENERGYCALC_H
