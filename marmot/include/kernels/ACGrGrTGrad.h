#ifndef ACGRGRTGRAD_H
#define ACGRGRTGRAD_H

#include "ACBulk.h"

//Forward Declarations
class ACGrGrTGrad;

template<>
InputParameters validParams<ACGrGrTGrad>();

/**
 * This kernel adds the temperature gradient driving force on a grain boundary.
 * From: Grain Boundary Migration in Metals, by Gottstein and Shvindlerman
 */
class ACGrGrTGrad : public ACBulk
{
public:
  ACGrGrTGrad(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

private:
  const MaterialProperty<Real> & _entropy_diff;
  const MaterialProperty<Real> & _molar_volume;
  const MaterialProperty<Real> & _act_wGB;
  unsigned int _T_var;
  VariableValue & _T;
  VariableGradient & _grad_T;
  Real _wGB;

  VariableValue & _c;
};

#endif //ACGRGRTGRAD_H
