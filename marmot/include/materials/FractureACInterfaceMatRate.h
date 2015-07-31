#ifndef FRACTUREACINTERFACEMATRATE_H
#define FRACTUREACINTERFACEMATRATE_H

#include "Material.h"

class FractureACInterfaceMatRate;

template<>
InputParameters validParams<FractureACInterfaceMatRate>();

class FractureACInterfaceMatRate : public Material
{
public:
  FractureACInterfaceMatRate(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _kappa_op;

  Real _L0;
  Real _l;
};

#endif //FRACTUREACINTERFACEMATRATE_H
