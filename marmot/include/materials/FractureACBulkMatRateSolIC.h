#ifndef FRACTUREACBULKMATRATESOLIC_H
#define FRACTUREACBULKMATRATESOLIC_H

#include "FractureACBulkMatRate.h"

class FractureACBulkMatRateSolIC;

template<>
InputParameters validParams<FractureACBulkMatRateSolIC>();

class FractureACBulkMatRateSolIC : public FractureACBulkMatRate
{
public:
  FractureACBulkMatRateSolIC(const InputParameters & parameters);

protected:
  virtual void initQpStatefulProperties() {};
  virtual void computeQpProperties();

private:
  VariableValue & _bnd;
  VariableValue & _conc;

  Real _gc_tens_gb;
  Real _gc_comp_gb;
  Real _gc_tens_mat;
  Real _gc_comp_mat;
  Real _gb_bnd;
};

#endif //FRACTUREACBULKMATRATESOLIC_H
