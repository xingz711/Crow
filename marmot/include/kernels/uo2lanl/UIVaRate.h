#ifndef UIVARATE_H
#define UIVARATE_H

#include "Kernel.h"

//Forward Declarations
class UIVaRate;

template<>
InputParameters validParams<UIVaRate>();

class UIVaRate : public Kernel
{
public:
  UIVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _MVa_name;
  const MaterialProperty<Real> & _MVa;

  std::string _MUI_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Zg;

  const MaterialProperty<Real> & _dgHVaUI;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cui;
  const MaterialProperty<Real> & _kappa_cu;
};

#endif //UIVARATE_H
