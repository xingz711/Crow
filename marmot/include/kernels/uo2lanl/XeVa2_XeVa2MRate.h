#ifndef XEVA2_XEVA2MRATE_H
#define XEVA2_XEVA2MRATE_H

#include "Kernel.h"

//Forward Declarations
class XeVa2_XeVa2MRate;

template<>
InputParameters validParams<XeVa2_XeVa2MRate>();

class XeVa2_XeVa2MRate : public Kernel
{
public:
  XeVa2_XeVa2MRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa2;

  //  std::string _mob_name2;
  //  const MaterialProperty<Real> & _MCgVa;

  //  std::string _mob_name3;
  //  const MaterialProperty<Real> & _MVa;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c6;
  VariableGradient & _grad_c6;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVa2;
  const MaterialProperty<Real> & _SBXeVa2;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgvv;
  //  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cg;
  const MaterialProperty<Real> & _kappa_cvv;
};

#endif //XEVA2_XEVA2MRATE_H
