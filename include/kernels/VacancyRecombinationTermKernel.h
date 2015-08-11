#ifndef VacancyRecombinationTermKernel_H
#define VacancyRecombinationTermKernel_H

#include "Kernel.h"

//Forward Declarations
class VacancyRecombinationTermKernel;

template<>
InputParameters validParams<VacancyRecombinationTermKernel>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class VacancyRecombinationTermKernel : public Kernel
{
public:
  VacancyRecombinationTermKernel(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  
private:
  
  VariableValue & _eta;
  VariableValue & _ci;
  
  Real _Rbulk;
  Real _Rs;

  
};

#endif //VacancyRecombinationTermKernel_h