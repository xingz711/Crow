#ifndef VASINKRATEGB_H
#define VASINKRATEGB_H

#include "Kernel.h"

//Forward Declarations
class VaSinkRateGB;

template<>
InputParameters validParams<VaSinkRateGB>();

class VaSinkRateGB : public Kernel
{
public:
  VaSinkRateGB(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vVaC_i;
  const MaterialProperty<std::vector<Real> > & _vVam_i;
  const MaterialProperty<std::vector<Real> > & _vVab_i;

  std::string _MVa_name;
  const MaterialProperty<Real> & _MVa;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  //  const MaterialProperty<Real> & _CU_old;
  //  const MaterialProperty<RealGradient> & _grad_CU_old;
  //  VariableValue & _u_old;
  //  VariableGradient & _grad_u_old;

  const MaterialProperty<Real> & _kT;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c4;
  VariableGradient & _grad_c4;

  // MaterialProperty<Real> & _Zg;

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
  //  const MaterialProperty<Real> & _kV2;

  unsigned int _n;
};

#endif //VASINKRATEGB_H
