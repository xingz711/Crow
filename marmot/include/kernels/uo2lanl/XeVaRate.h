#ifndef XEVARATE_H
#define XEVARATE_H

#include "Kernel.h"

//Forward Declarations
class XeVaRate;

template<>
InputParameters validParams<XeVaRate>();

class XeVaRate : public Kernel
{
public:
  XeVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

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

#endif //XEVARATE_H
