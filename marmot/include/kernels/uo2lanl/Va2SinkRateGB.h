#ifndef VA2SINKRATEGB_H
#define VA2SINKRATEGB_H

#include "Kernel.h"

//Forward Declarations
class Va2SinkRateGB;

template<>
InputParameters validParams<Va2SinkRateGB>();

class Va2SinkRateGB : public Kernel
{
public:
  Va2SinkRateGB(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vVaC_i;
  const MaterialProperty<std::vector<Real> > & _vVam_i;
  const MaterialProperty<std::vector<Real> > & _vVab_i;

  std::string _MVa2_name;
  const MaterialProperty<Real> & _MVa2;

  const MaterialProperty<Real> & _CU;
  const MaterialProperty<RealGradient> & _grad_CU;

  const MaterialProperty<Real> & _kT;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c2;
  VariableGradient & _grad_c2;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  // MaterialProperty<Real> & _Zg;

  const MaterialProperty<Real> & _dgHVa2;
  const MaterialProperty<Real> & _dgSVa2;
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
  // MaterialProperty<Real> & _kV2;

  unsigned int _n;
};

#endif //VA2SINKRATEGB_H
