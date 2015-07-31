#ifndef MATERIALPROPERTYVALUE_H
#define MATERIALPROPERTYVALUE_H

#include "KernelValue.h"

//Forward Declarations
class MaterialPropertyValue;

template<>
InputParameters validParams<MaterialPropertyValue>();

class MaterialPropertyValue : public KernelValue
{
public:
  MaterialPropertyValue(const InputParameters & parameters);

protected:
  virtual Real precomputeQpResidual();
  virtual Real precomputeQpJacobian();

  Real _kernel_sign;
  const MaterialProperty<Real> & _prop;
};

#endif //MATERIALPROPERTYVALUE_H
