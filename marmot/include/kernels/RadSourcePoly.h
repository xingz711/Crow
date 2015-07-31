#ifndef RADSOURCEPOLY_H
#define RADSOURCEPOLY_H

#include "KernelValue.h"

//Forward Declarations
class RadSourcePoly;

template<>
InputParameters validParams<RadSourcePoly>();

/**
 * Vacancy or interstitial source term
 * \todo remove the ugly bool parameter and apply whichever material property is passed in
 */
class RadSourcePoly : public KernelValue
{
public:
  RadSourcePoly(const InputParameters & parameters);

protected:
  virtual Real precomputeQpResidual();
  virtual void subdomainSetup();

private:
  bool _Vac_or_Int;

  const MaterialProperty<Real> & _vacancy_increase;
  const MaterialProperty<Real> & _interstitial_increase;
};

#endif //RADSOURCEPOLY_H
