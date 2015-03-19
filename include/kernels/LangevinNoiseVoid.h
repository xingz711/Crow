#ifndef LANGEVINNOISEVOID_H
#define LANGEVINNOISEVOID_H

#include "Kernel.h"

//Forward Declarations
class LangevinNoiseVoid;

template<>
InputParameters validParams<LangevinNoiseVoid>();

class LangevinNoiseVoid : public Kernel
{
public:
  LangevinNoiseVoid(const std::string & name, InputParameters parameters);

protected:
  virtual void residualSetup();
  virtual Real computeQpResidual();

  const Real _amplitude;
  const MaterialProperty<Real> * _multiplier_prop;
  
private:
  VariableValue & _eta;
  Real _min;
  Real _max;
  Real _range;
};

#endif //LANGEVINNOISEVOID_H
