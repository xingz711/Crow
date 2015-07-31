#ifndef VOIDTEMPGRAD_H
#define VOIDTEMPGRAD_H

#include "Material.h"

//Forward Declarations
class VoidTempGrad;

template<>
InputParameters validParams<VoidTempGrad>();

class VoidTempGrad : public Material
{
public:
  VoidTempGrad(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _temp;
  VariableGradient & _grad_temp;

  MaterialProperty<Real> & _Dvan;
  // MaterialProperty<RealGradient> & _grad_Dvan;
  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _Qheat;
  MaterialProperty<Real> & _mobtemp;

  Real _Dvan_zero;
  Real _Act_energy;
  Real _Heat_trans;
};

#endif //VOIDTEMPGRAD_H
