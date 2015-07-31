#ifndef SPLITCHZRH_H
#define SPLITCHZRH_H

#include "SplitCHCRes.h"


//Forward Declarations
class SplitCHZrH;

template<>
InputParameters validParams<SplitCHZrH>();


/**
 * Calculates the C residual term for the gamma Zr-H model
 */
class SplitCHZrH : public SplitCHCRes
{
public:
  SplitCHZrH(const InputParameters & parameters);

protected:
  virtual Real computeDFDC(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  VariableValue & _eta1;
  unsigned int _eta1_var;
  VariableValue & _eta2;
  unsigned int _eta2_var;
  VariableValue & _eta3;
  unsigned int _eta3_var;

  const MaterialProperty<Real> & _G;
  const Real _b;
  const Real _c;
  const Real _a;
  const Real _C_m;
  const Real _C_gamma;

};

#endif //SPLITCHZRH_H
