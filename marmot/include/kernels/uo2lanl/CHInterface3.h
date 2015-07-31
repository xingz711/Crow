#ifndef CHINTERFACE3_H
#define CHINTERFACE3_H

#include "Kernel.h"

//Forward Declarations
class CHInterface3;

template<>
InputParameters validParams<CHInterface3>();

class CHInterface3 : public Kernel
{
public:
  CHInterface3(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _M;
  const MaterialProperty<RealGradient> & _grad_M;

  VariableValue & _c1;
  VariableGradient & _grad_c1;
  VariableSecond & _second_c1;

  VariableValue & _c2;
  VariableGradient & _grad_c2;
  VariableSecond & _second_c2;

  VariableValue & _c3;
  VariableGradient & _grad_c3;
  VariableSecond & _second_c3;

  VariableValue & _c4;
  VariableGradient & _grad_c4;
  VariableSecond & _second_c4;

  VariableTestSecond & _second_test;
  VariablePhiSecond & _second_phi;
};

#endif //CHINTERFACE3_H
