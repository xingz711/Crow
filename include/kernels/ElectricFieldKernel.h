#ifndef ELECTRICFIELDKERNEL_H
#define ELECTRICFIELDKERNEL_H

#include "HeatSource.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class ElectricFieldKernel;

template<>
InputParameters validParams<ElectricFieldKernel>();

/**
 * This kernel calculates the heat source term corresponding to joule heating,
 * Q = J * E = elec_cond * grad_phi * grad_phi, where phi is the electrical potenstial.
 */
class ElectricFieldKernel : public DerivativeMaterialInterface<JvarMapKernelInterface<HeatSource> >
{
public:
  ElectricFieldKernel(const InputParameters & parameters);
  virtual void initialSetup();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const VariableGradient & _grad_elec;
  const unsigned int _elec_var;

  const MaterialProperty<Real> & _elec_cond;
  const MaterialProperty<Real> & _delec_cond_dT;
  std::vector<const MaterialProperty<Real> *> _delec_cond_darg;
};

#endif //ELECTRICFIELDKERNEL_H
