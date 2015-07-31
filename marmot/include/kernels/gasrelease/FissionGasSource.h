#ifndef FISSIONGASSOURCE_H
#define FISSIONGASSOURCE_H

#include "Kernel.h"

//Forward Declarations
class FissionGasSource;

template<>
InputParameters validParams<FissionGasSource>();

class FissionGasSource : public Kernel
{
public:
  FissionGasSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _a_name;
  const MaterialProperty<Real> & _a;

  Real _b;
};

#endif //UISOURCE_H
