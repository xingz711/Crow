#ifndef UO2ANISOCONSTEMP_H
#define UO2ANISOCONSTEMP_H

#include "Material.h"

//Forward Declarations
class UO2AnisoConsTemp;

template<>
InputParameters validParams<UO2AnisoConsTemp>();

class UO2AnisoConsTemp : public Material
{
public:
  UO2AnisoConsTemp(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  VariableValue & _c;
  VariableGradient & _grad_c;

  Real _temp;
  Real _kappa_cg;
  Real _f0s;
  // Real _wGB;
  Real _M0GB;
  Real _QGB;
  Real _sigma_GB;

  MaterialProperty<Real> & _Dg;
  MaterialProperty<Real> & _Mg;
  MaterialProperty<RealGradient> & _grad_Mg;

  MaterialProperty<Real> & _Ef;
  MaterialProperty<RealGradient> & _grad_Ef;

  MaterialProperty<Real> & _kT;
  MaterialProperty<std::vector<Real> > & _vC_i;
  MaterialProperty<std::vector<Real> > & _vm_i;
  MaterialProperty<std::vector<Real> > & _vb_i;
  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _kappa_GB;
  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _mu;
  MaterialProperty<Real> & _tgrad_corr_mult;

  unsigned int _n;
};

#endif //UO2ANISOCONSTEMP_H
