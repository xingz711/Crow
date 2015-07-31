#ifndef UI_XEVA2UIRATE_H
#define UI_XEVA2UIRATE_H

#include "Kernel.h"

//Forward Declarations
class UI_XeVa2UIRate;

template<>
InputParameters validParams<UI_XeVa2UIRate>();

class UI_XeVa2UIRate : public Kernel
{
public:
  UI_XeVa2UIRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _MXeVa2_name;
  const MaterialProperty<Real> & _MXeVa2;

  std::string _MUI_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c6;
  VariableGradient & _grad_c6;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Zg;
  const MaterialProperty<Real> & _dgHXeVa2UI;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgv;
  const MaterialProperty<Real> & _kappa_cgvv;
  const MaterialProperty<Real> & _kappa_cui;
  const MaterialProperty<Real> & _kappa_cu;
};

#endif //UI_XEVA2UIRATE_H
