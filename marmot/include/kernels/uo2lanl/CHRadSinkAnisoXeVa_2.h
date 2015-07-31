#ifndef CHRADSINKANISOXEVA_2_H
#define CHRADSINKANISOXEVA_2_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class CHRadSinkAnisoXeVa_2;

template<>
InputParameters validParams<CHRadSinkAnisoXeVa_2>();

class CHRadSinkAnisoXeVa_2 : public Kernel
{
public:
  CHRadSinkAnisoXeVa_2(const InputParameters & parameters);

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

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};
#endif //CHRADSINKANISOXEVA_2_H
