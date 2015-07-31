#ifndef ACFRACINTVAR_H
#define ACFRACINTVAR_H

#include "KernelValue.h"

//Forward Declarations
class ACFracIntVar;

template<>
InputParameters validParams<ACFracIntVar>();

/**
 * Formulation: Miehe et. al., Int. J. Num. Methods Engg., 2010, 83. 1273-1311
 * Equation 63
 */
class ACFracIntVar : public KernelValue
{
public:
  ACFracIntVar(const InputParameters & parameters);

protected:
  VariableValue & _c;

  virtual Real precomputeQpResidual();
  virtual Real precomputeQpJacobian();
};

#endif //ACFRACINTVAR_H
