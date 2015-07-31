#ifndef ORDERPARAMINTERFACE_H
#define ORDERPARAMINTERFACE_H

#include "KernelGrad.h"

//Forward Declarations
class OrderParamInterface;

template<>
InputParameters validParams<OrderParamInterface>();

class OrderParamInterface : public KernelGrad
{
public:
  OrderParamInterface(const InputParameters & parameters);

protected:
  virtual RealGradient precomputeQpResidual();
  virtual RealGradient precomputeQpJacobian();
  virtual RealGradient precomputeQpOffDiagJacobian(unsigned int jvar);

private:
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _L;
  VariableValue & _eta2;
  VariableGradient & _grad_eta2;
  VariableValue & _eta3;
  VariableGradient & _grad_eta3;
  unsigned int _eta2_var;
  unsigned int _eta3_var;
};

#endif //ORDERPARAMINTERFACE_H
