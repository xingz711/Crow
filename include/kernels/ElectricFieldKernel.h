#ifndef ELECTRICFIELDKERNEL_H
#define ELECTRICFIELDKERNEL_H

#include "Kernel.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class ElectricFieldKernel;

template<>
InputParameters validParams<ElectricFieldKernel>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class ElectricFieldKernel : public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel> >
{
public:
  ElectricFieldKernel(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int javar);

private:
  const VariableValue & _elec;
  const VariableGradient & _grad_elec;
  unsigned int _elec_var;

  const MaterialProperty<Real> & _electic_conductivity;
  const MaterialProperty<Real> & _dRdT;
  std::vector<const MaterialProperty<Real> *> _dRdarg;

};

#endif //ELECTRICFIELDKERNEL_H
