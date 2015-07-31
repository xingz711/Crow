#ifndef VAUIRATE_H
#define VAUIRATE_H

#include "Kernel.h"

//Forward Declarations
class VaUIRate;

template<>
InputParameters validParams<VaUIRate>();

class VaUIRate : public Kernel
{
public:
  VaUIRate(const InputParameters & parameters);

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

  VariableValue & _c5;
  VariableGradient & _grad_c5;

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

#endif //VAUIRATE_H
