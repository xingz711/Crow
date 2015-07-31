#ifndef UIVA2RATE_H
#define UIVA2RATE_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class UIVa2Rate;

template<>
InputParameters validParams<UIVa2Rate>();

class UIVa2Rate : public Kernel
{
public:
  UIVa2Rate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  std::string _MVa2_name;
  const MaterialProperty<Real> & _MVa2;

  std::string _MUI_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Zg;

  const MaterialProperty<Real> & _dgHVaUI;
  const MaterialProperty<Real> & _dgHVa;
  const MaterialProperty<Real> & _dgSVa;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cvv;
  const MaterialProperty<Real> & _kappa_cui;
  const MaterialProperty<Real> & _kappa_cu;
};

#endif //UIVA2RATE_H
