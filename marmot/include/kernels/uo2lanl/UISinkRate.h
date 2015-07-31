#ifndef UISINKRATE_H
#define UISINKRATE_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class UISinkRate;

template<>
InputParameters validParams<UISinkRate>();

class UISinkRate : public Kernel
{
public:
  UISinkRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _MUI_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _Zg;

  const MaterialProperty<Real> & _dgHVaUI;
  const MaterialProperty<Real> & _dgHVa;
  const MaterialProperty<Real> & _dgSVa;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cvv;
  const MaterialProperty<Real> & _kappa_cui;
  const MaterialProperty<Real> & _kappa_cu;

  const MaterialProperty<Real> & _kI2;
};

#endif //UISINKRATE_H
