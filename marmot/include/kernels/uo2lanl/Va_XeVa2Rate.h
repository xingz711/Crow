#ifndef VA_XEVA2RATE_H
#define VA_XEVA2RATE_H

#include "Kernel.h"

//Forward Declarations
class Va_XeVa2Rate;

template<>
InputParameters validParams<Va_XeVa2Rate>();

class Va_XeVa2Rate : public Kernel
{
public:
  Va_XeVa2Rate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa2;

  std::string _mob_name2;
  const MaterialProperty<Real> & _MCgVa;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVa;
  const MaterialProperty<Real> & _SBXeVa;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgv;
  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cg;
  const MaterialProperty<Real> & _kappa_cvv;
};

#endif //VA_XEVA2RATE_H
