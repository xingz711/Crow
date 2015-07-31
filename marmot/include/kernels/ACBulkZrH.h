#ifndef ACBULKZrH_H
#define ACBULKZrH_H

#include "ACBulk.h"

//Forward Declarations
class ACBulkZrH;

template<>
InputParameters validParams<ACBulkZrH>();

class ACBulkZrH : public ACBulk
{
public:
  ACBulkZrH(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  VariableValue & _eta2;
  unsigned int _eta2_var;
  VariableValue & _eta3;
  unsigned int _eta3_var;
  VariableValue & _C;
  unsigned int _C_var;

  const MaterialProperty<Real> & _G;
  const Real _b;
  const Real _c;
  const Real _d;
  const Real _C_m;
  const Real _C_gamma;
};

#endif //ACBULKZrH_H
