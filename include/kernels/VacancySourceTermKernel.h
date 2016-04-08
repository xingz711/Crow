#ifndef VACANCYSOURCETERMKERNEL_H
#define VACANCYSOURCETERMKERNEL_H

#include "Kernel.h"

//Forward Declarations
class VacancySourceTermKernel;

template<>
InputParameters validParams<VacancySourceTermKernel>();

/**Cahn-Hilliard Kernel implementing the free energy for sintering
 **/
class VacancySourceTermKernel : public Kernel
{
public:
  VacancySourceTermKernel(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
 // virtual Real computeQpJacobian();

private:
  const VariableValue & _eta;
  //VariableValue & _R1;
 // VariableValue & _R2;

  std::string _rand_num;
  const MaterialProperty<Real> & _R;
  //Real _Pcasc;
  Real _Vg;
};

#endif //VACANCYSOURCETERMKERNEL_H
