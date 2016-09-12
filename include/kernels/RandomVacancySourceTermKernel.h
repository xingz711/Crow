#ifndef RANDOMVACANCYSOURCETERMKERNEL_H
#define RANDOMVACANCYSOURCETERMKERNEL_H

#include "Kernel.h"

//Forward Declarations
class RandomVacancySourceTermKernel;

template<>
InputParameters validParams<RandomVacancySourceTermKernel>();

/**Cahn-Hilliard Kernel implementing the free energy for sintering
 **/
class RandomVacancySourceTermKernel : public Kernel
{
public:
  RandomVacancySourceTermKernel(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
 // virtual Real computeQpJacobian();

private:
  const VariableValue & _eta;
  Real _min;
  Real _max;
   //Real _Pcasc;
  Real _Vg;
  Real _range;
};

#endif //VACANCYSOURCETERMKERNEL_H
