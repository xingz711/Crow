#ifndef VA_XEXEVARATE_H
#define VA_XEXEVARATE_H

#include "Kernel.h"

//Forward Declarations
class Va_XeXeVaRate;

template<>
InputParameters validParams<Va_XeXeVaRate>();

class Va_XeXeVaRate : public Kernel
{
public:
  Va_XeXeVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  //  const MaterialProperty<Real> & _MXe;
  const MaterialProperty<Real> & _MXeXe;

  //  std::string _mob_name2;
  //  const MaterialProperty<Real> & _MCgVa;

  //  std::string _mob_name3;
  //  const MaterialProperty<Real> & _MVa;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c6;
  VariableGradient & _grad_c6;

  VariableValue & _c7;
  VariableGradient & _grad_c7;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeXeVa;
  const MaterialProperty<Real> & _SBXeXeVa;
  //  const MaterialProperty<Real> & _EBVaVa;
  //  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cggv;
  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cg;
  const MaterialProperty<Real> & _kappa_cgvv;
};

#endif //XEVA2_XEVA2RATE_H
