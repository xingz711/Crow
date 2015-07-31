#ifndef VAVARATE_H
#define VAVARATE_H

#include "Kernel.h"

//Forward Declarations
class VaVaRate;

template<>
InputParameters validParams<VaVaRate>();

class VaVaRate : public Kernel
{
public:
  VaVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  const MaterialProperty<Real> & _MVa;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBVaVa;
  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cvv;
  const MaterialProperty<Real> & _kappa_cv;
};

#endif //VAVARATE_H
