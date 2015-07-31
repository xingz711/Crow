#ifndef CHRADSINKANISOVA_H
#define CHRADSINKANISOVA_H

#include "Kernel.h"

//Forward Declarations
class CHRadSinkAnisoVa;

template<>
InputParameters validParams<CHRadSinkAnisoVa>();

class CHRadSinkAnisoVa : public Kernel
{
public:
  CHRadSinkAnisoVa(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vVaC_i;
  const MaterialProperty<std::vector<Real> > & _vVam_i;
  const MaterialProperty<std::vector<Real> > & _vVab_i;

  std::string _mob_name;
  const MaterialProperty<Real> & _M;

  const MaterialProperty<Real> & _kb;
  const MaterialProperty<Real> & _kT;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};

#endif //CHRADSINKANISOVA_H
