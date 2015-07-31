#ifndef VARTEMPIRRAD_H
#define VARTEMPIRRAD_H

#include "Material.h"

//Forward Declarations
class VarTempIrrad;

template<>
InputParameters validParams<VarTempIrrad>();

class VarTempIrrad : public Material
{
public:
  VarTempIrrad(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _temp;
  VariableGradient & _grad_temp;

  VariableValue & _c;
  VariableGradient & _grad_c;

  MaterialProperty<Real> & _M_v;
  MaterialProperty<RealGradient> & _grad_M_v;
  MaterialProperty<Real> & _M_i;

  MaterialProperty<Real> & _Efv;
  MaterialProperty<Real> & _Efi;

  MaterialProperty<Real> & _Emv;
  MaterialProperty<Real> & _Emi;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _L_eta;
  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_cv;

  Real _kappa_cv_input;
};

#endif //VARTEMPIRRAD_H
