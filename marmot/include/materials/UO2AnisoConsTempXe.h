#ifndef UO2ANISOCONSTEMPXE_H
#define UO2ANISOCONSTEMPXE_H

#include "Material.h"

//Forward Declarations
class UO2AnisoConsTempXe;

template<>
InputParameters validParams<UO2AnisoConsTempXe>();

class UO2AnisoConsTempXe : public Material
{
public:
  UO2AnisoConsTempXe(const InputParameters & parameters);

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
  Real _tstop;
  // Real _wGB;
  Real _M0GB;
  Real _QGB;
  Real _sigma_GB;
  // Real _L0;
  Real _HXe;
  Real _D0Xe;

  // MaterialProperty<Real> & _Lg;
  MaterialProperty<Real> & _Dg;
  MaterialProperty<Real> & _Mg;
  MaterialProperty<Real> & _Mgm;
  MaterialProperty<RealGradient> & _grad_Mg;
  MaterialProperty<RealGradient> & _grad_Mgm;

  // MaterialProperty<Real> & _Ef;
  // MaterialProperty<RealGradient> & _grad_Ef;
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

  MaterialProperty<Real> & _LogC;
  MaterialProperty<Real> & _LogTol;

  MaterialProperty<Real> & _tgrad_corr_mult;

  unsigned int _n;
};

#endif //UO2ANISOCONSTEMPXE_H
