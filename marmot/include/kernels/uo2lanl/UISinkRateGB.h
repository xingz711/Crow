#ifndef UISINKRATEGB_H
#define UISINKRATEGB_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class UISinkRateGB;

template<>
InputParameters validParams<UISinkRateGB>();

class UISinkRateGB : public Kernel
{
public:

  UISinkRateGB(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vUIC_i;
  const MaterialProperty<std::vector<Real> > & _vUIm_i;
  const MaterialProperty<std::vector<Real> > & _vUIb_i;

  std::string _MUI_name;
  const MaterialProperty<Real> & _MUI;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  const MaterialProperty<Real> & _kT;
  // MaterialProperty<Real> & _Zg;

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

  // MaterialProperty<Real> & _kI2;
  unsigned int _n;
};

#endif //UISINKRATEGB_H
