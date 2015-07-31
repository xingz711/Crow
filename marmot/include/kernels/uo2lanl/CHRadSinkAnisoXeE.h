#ifndef CHRADSINKANISOXEE_H
#define CHRADSINKANISOXEE_H

#include "Kernel.h"

//Forward Declarations
class CHRadSinkAnisoXeE;

template<>
InputParameters validParams<CHRadSinkAnisoXeE>();

class CHRadSinkAnisoXeE : public Kernel
{
public:

  CHRadSinkAnisoXeE(const InputParameters & parameters);

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

  //  VariableValue & _c1Old;
  //  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  //  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};

#endif //CHRADSINKANISOXEE_H
