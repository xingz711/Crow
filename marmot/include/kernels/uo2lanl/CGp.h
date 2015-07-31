#ifndef CGP_H
#define CGP_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class CGp;

template<>
InputParameters validParams<CGp>();

class CGp : public Kernel
{
public:
  CGp(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  //  std::string _K_name;
  //  const MaterialProperty<Real> & _K;

  VariableValue & _c1;
  VariableValue & _c2;
  VariableValue & _c3;
  VariableValue & _c4;
  VariableValue & _c6;

  const MaterialProperty<Real> & _EBXeVa;
  const MaterialProperty<Real> & _SBXeVa;
  const MaterialProperty<Real> & _kT;
};

#endif //CGP_H
