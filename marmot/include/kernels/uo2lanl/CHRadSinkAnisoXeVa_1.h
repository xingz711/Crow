#ifndef CHRADSINKANISOXEVA_1_H
#define CHRADSINKANISOXEVA_1_H

#include "Kernel.h"

//Forward Declarations
class CHRadSinkAnisoXeVa_1;

template<>
InputParameters validParams<CHRadSinkAnisoXeVa_1>();

class CHRadSinkAnisoXeVa_1 : public Kernel
{
public:
  CHRadSinkAnisoXeVa_1(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vC_i;
  const MaterialProperty<std::vector<Real> > & _vm_i;
  const MaterialProperty<std::vector<Real> > & _vb_i;

  std::string _mob_name;
  const MaterialProperty<Real> & _M;

  const MaterialProperty<Real> & _kb;
  const MaterialProperty<Real> & _kT;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};

#endif //CHRADSINKANISOXEVA_1_H
