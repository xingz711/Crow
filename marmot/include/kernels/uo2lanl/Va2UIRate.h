#ifndef VA2UIRATE_H
#define VA2UIRATE_H

#include "Kernel.h"

//Forward Declarations
class Va2UIRate;

template<>
InputParameters validParams<Va2UIRate>();

class Va2UIRate : public Kernel
{
public:
  Va2UIRate(const InputParameters & parameters);

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

  VariableValue & _c5;
  VariableGradient & _grad_c5;

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

#endif //VA2UIRATE_H
