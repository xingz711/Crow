#ifndef VA_XEVA2BRATE_H
#define VA_XEVA2BRATE_H

#include "Kernel.h"

//Forward Declarations
class Va_XeVa2BRate;

template<>
InputParameters validParams<Va_XeVa2BRate>();

class Va_XeVa2BRate : public Kernel
{
public:
  Va_XeVa2BRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa;

  std::string _mob_name2;
  const MaterialProperty<Real> & _MCgVa;

  //  std::string _mob_name3;
  //  const MaterialProperty<Real> & _MVa;

  VariableValue & _c6;
  VariableGradient & _grad_c6;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVa2B;
  const MaterialProperty<Real> & _SBXeVa2B;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgvv;
  //  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cgv;
  const MaterialProperty<Real> & _kappa_cv;
};

#endif //VA_XEVA2BRATE_H
