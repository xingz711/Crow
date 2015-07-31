#ifndef CHRADSINKANISOVAVA_H
#define CHRADSINKANISOVAVA_H

#include "Kernel.h"

//Forward Declarations
class CHRadSinkAnisoVaVa;

template<>
InputParameters validParams<CHRadSinkAnisoVaVa>();

class CHRadSinkAnisoVaVa : public Kernel
{
public:
  CHRadSinkAnisoVaVa(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vVaC_i;
  const MaterialProperty<std::vector<Real> > & _vVam_i;
  const MaterialProperty<std::vector<Real> > & _vVab_i;

  const MaterialProperty<Real> & _M;
  const MaterialProperty<Real> & _kb;
  const MaterialProperty<Real> & _kT;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  const MaterialProperty<Real> & _LogC;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};

#endif //CHRADSINKANISOVAVA_H
