#ifndef VA_XEVARATE_H
#define VA_XEVARATE_H

#include "Kernel.h"

//Forward Declarations
class Va_XeVaRate;

template<>
InputParameters validParams<Va_XeVaRate>();

class Va_XeVaRate : public Kernel
{
public:
  Va_XeVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVa;
  const MaterialProperty<Real> & _SBXeVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgv;
  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cg;
};

#endif //VA_XEVARATE_H
