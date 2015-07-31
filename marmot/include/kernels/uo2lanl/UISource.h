#ifndef UISOURCE_H
#define UISOURCE_H

#include "Kernel.h"

//Forward Declarations
class UISource;

template<>
InputParameters validParams<UISource>();

class UISource : public Kernel
{
public:
  UISource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Real _R;
  Real _F;
};

#endif //UISOURCE_H
