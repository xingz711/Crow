#ifndef XEVA_XEVAXEVARATE_H
#define XEVA_XEVAXEVARATE_H

#include "Kernel.h"

//Forward Declarations
class XeVa_XeVaXeVaRate;

template<>
InputParameters validParams<XeVa_XeVaXeVaRate>();

class XeVa_XeVaXeVaRate : public Kernel
{
public:
  XeVa_XeVaXeVaRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::string _mob_name;
  //  const MaterialProperty<Real> & _MXe;
  const MaterialProperty<Real> & _MXe;

  //  std::string _mob_name2;
  //  const MaterialProperty<Real> & _MCgVa;

  //  std::string _mob_name3;
  //  const MaterialProperty<Real> & _MVa;

  VariableValue & _c3;
  VariableGradient & _grad_c3;

  VariableValue & _c7;
  VariableGradient & _grad_c7;

  //  VariableValue & _c2;
  //  VariableGradient & _grad_c2;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVaXeVa;
  const MaterialProperty<Real> & _SBXeVaXeVa;
  //  const MaterialProperty<Real> & _EBVaVa;
  //  const MaterialProperty<Real> & _SBVaVa;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cggv;
  //  const MaterialProperty<Real> & _kappa_cv;
  //  const MaterialProperty<Real> & _kappa_cg;
  const MaterialProperty<Real> & _kappa_cgv;
};

#endif //XEVA2_XEVA2RATE_H
