#ifndef CHRADSINKANISO_H
#define CHRADSINKANISO_H

#include "CHBulk.h"

//Forward Declarations
class CHRadSinkAniso;

template<>
InputParameters validParams<CHRadSinkAniso>();

class CHRadSinkAniso : public CHBulk
{
public:
  CHRadSinkAniso(const InputParameters & parameters);

protected:
  virtual RealGradient computeGradDFDCons(PFFunctionType type);

  const MaterialProperty<std::vector<Real> > & _vC_i;
  const MaterialProperty<std::vector<Real> > & _vm_i;
  const MaterialProperty<std::vector<Real> > & _vb_i;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  unsigned int _n;
};

#endif //CHRADSINKANISO_H
