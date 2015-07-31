#ifndef CHINTERFACEMMS_H
#define CHINTERFACEMMS_H

#include "Kernel.h"

//Forward Declarations
class CHInterfaceMMS;

template<>
InputParameters validParams<CHInterfaceMMS>();

class CHInterfaceMMS : public Kernel
{
public:
  CHInterfaceMMS(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  VariableSecond & _second_u;
  VariableTestSecond & _second_test;
  VariablePhiSecond & _second_phi;
};

#endif //CHINTERFACEMMS_H
