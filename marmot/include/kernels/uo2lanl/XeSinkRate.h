#ifndef XESINKRATE_H
#define XESINKRATE_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class XeSinkRate;

template<>
InputParameters validParams<XeSinkRate>();

class XeSinkRate : public Kernel
{
public:

  XeSinkRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:

  std::string _MXe_name;

  const MaterialProperty<Real> & _MXe;

  const MaterialProperty<Real> & _CU;

  const MaterialProperty<RealGradient> & _grad_CU;

  //  const MaterialProperty<Real> & _CU_old;

  //  const MaterialProperty<RealGradient> & _grad_CU_old;

  //  VariableValue & _u_old;

  //  VariableGradient & _grad_u_old;

  const MaterialProperty<Real> & _kT;

  const MaterialProperty<Real> & _Zg;

  //  const MaterialProperty<Real> & _dgHVa;

  //  const MaterialProperty<Real> & _dgSVa;

  std::string _LogC_name;

  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;

  const MaterialProperty<Real> & _LogTol;

  //  const MaterialProperty<Real> & _kappa_cv;

  //  const MaterialProperty<Real> & _kappa_cvv;

  //  const MaterialProperty<Real> & _kappa_cui;

  //  const MaterialProperty<Real> & _kappa_cu;

  //  const MaterialProperty<Real> & _kV2;
};

#endif //XESINKRATE_H
