#ifndef CHTGRAD_H
#define CHTGRAD_H

#include "CHBulk.h"

//Forward Declarations
class CHTGrad;

template<>
InputParameters validParams<CHTGrad>();

class CHTGrad : public CHBulk
{
public:
  CHTGrad(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

private:
  const MaterialProperty<Real> & _Qstar;
  VariableValue & _T;
  VariableGradient & _grad_T;
};

#endif //CHTGRAD_H
