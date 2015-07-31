#ifndef CHVACWGAS_H
#define CHVACWGAS_H

#include "CHBulk.h"

//Forward Declarations
class CHVacwGas;

template<>
InputParameters validParams<CHVacwGas>();

class CHVacwGas : public CHBulk
{
public:
  CHVacwGas(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  VariableValue & _cg;
  VariableGradient & _grad_cg;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Efv;
  const MaterialProperty<Real> & _Efg;
  const MaterialProperty<Real> & _mug0;
};

#endif //CHVACWGAS_H
