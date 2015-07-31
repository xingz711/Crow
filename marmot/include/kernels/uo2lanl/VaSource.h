#ifndef VASOURCE_H
#define VASOURCE_H

#include "Kernel.h"

//Forward Declarations
class VaSource;

template<>
InputParameters validParams<VaSource>();

class VaSource : public Kernel
{
public:
  VaSource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real _R;
  Real _F;
};

#endif //VASOURCE_H
