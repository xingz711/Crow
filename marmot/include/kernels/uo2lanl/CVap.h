#ifndef CVAP_H
#define CVAP_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class CVap;

template<>
InputParameters validParams<CVap>();

class CVap : public Kernel
{
public:
  CVap(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  //  std::string _K_name;
  //  const MaterialProperty<Real> & _K;

  VariableValue & _c1;
  VariableValue & _c2;
  VariableValue & _c3;
  VariableValue & _c4;
  VariableValue & _c7;

  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _kT;
};

#endif //CVAP_H
