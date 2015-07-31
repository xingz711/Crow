#ifndef Va2_XeVa2_BRate_H
#define Va2_XeVa2_BRate_H

#include "Kernel.h"
#include "Material.h"

//Forward Declarations
class Va2_XeVa2_BRate;

template<>
InputParameters validParams<Va2_XeVa2_BRate>();

class Va2_XeVa2_BRate : public Kernel
{
public:
  Va2_XeVa2_BRate(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();

private:
  std::string _mob_name;

  const MaterialProperty<Real> & _MVa2;

  VariableValue & _c1;
  VariableGradient & _grad_c1;

  VariableValue & _c6;
  VariableGradient & _grad_c6;

  const MaterialProperty<Real> & _kT;
  const MaterialProperty<Real> & _EBXeVa2;
  const MaterialProperty<Real> & _SBXeVa2;
  const MaterialProperty<Real> & _Zg;

  std::string _LogC_name;
  const MaterialProperty<Real> & _LogC;

  std::string _LogTol_name;
  const MaterialProperty<Real> & _LogTol;

  const MaterialProperty<Real> & _kappa_cgv;
  const MaterialProperty<Real> & _kappa_cv;
  const MaterialProperty<Real> & _kappa_cg;
};

#endif //COEFDIFFUSION_H
