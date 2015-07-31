#ifndef CHGASWVAC_H
#define CHGASWVAC_H

#include "CHBulk.h"

//Forward Declarations
class CHGaswVac;

template<>
InputParameters validParams<CHGaswVac>();

/**
 * This kernel calculates the bulk free energy term from the Cahn-hilliard
 */
class CHGaswVac : public CHBulk
{
public:
  CHGaswVac(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  VariableValue & _cv;
  VariableGradient & _grad_cv;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Efg;
  const MaterialProperty<Real> & _mug0;
};

#endif //CHGASWVAC_H
