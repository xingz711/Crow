#ifndef UO2ANISOCONSTEMPXEUVAB_H
#define UO2ANISOCONSTEMPXEUVAB_H

#include "Material.h"

//Forward Declarations
class UO2AnisoConsTempXeUVaB;

template<>
InputParameters validParams<UO2AnisoConsTempXeUVaB>();

class UO2AnisoConsTempXeUVaB : public Material
{
public:
  UO2AnisoConsTempXeUVaB(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  std::vector<VariableValue *> _valsVa;
  std::vector<VariableGradient *> _grad_valsVa;

  VariableValue & _c1;
  VariableValue & _c2;
  VariableValue & _c3;
  VariableValue & _c4;
  VariableValue & _c6;
  //  VariableValue & _c1_old;
  //  VariableValue & _c2_old;
  //  VariableValue & _c3_old;
  //  VariableValue & _c4_old;
  VariableGradient & _grad_c1;
  VariableGradient & _grad_c2;
  VariableGradient & _grad_c3;
  VariableGradient & _grad_c4;
  VariableGradient & _grad_c6;
  //  VariableGradient & _grad_c1_old;
  //  VariableGradient & _grad_c2_old;
  //  VariableGradient & _grad_c3_old;
  //  VariableGradient & _grad_c4_old;


  Real _temp;
  Real _kappa_cg;
  Real _kappa_cu;
  Real _kappa_cv;
  Real _kappa_cgv;
  Real _kappa_cvv;
  Real _kappa_cui;

  Real _ki2;
  Real _kv2;

  Real _f0s;
  // Real _wGB;
  Real _M0GB;
  Real _QGB;
  Real _sigma_GB;
  Real _LXeVa;
  Real _LUVa;
  Real _LXeU;
  Real _LXeVaU;
  Real _LXeVaVa;
  Real _LXeVaXe;
  Real _LXeVa2;
  Real _LXeVaVa2;
  Real _LUVa2;
  Real _LVaVa2;
  Real _LTXeVa;
  Real _LTUVa;
  Real _LTXeU;
  Real _LTXeVaU;
  Real _LTXeVaVa;
  Real _LTXeVaXe;
  Real _LTXeVa2;
  Real _LTXeVaVa2;
  Real _LTUVa2;
  Real _LTVaVa2;
  Real _EBXeVa;
  Real _EBXeVa2;
  Real _EBVaVa;
  Real _SBXeVa;
  Real _SBXeVa2;
  Real _SBVaVa;
  Real _HXe;
  Real _D0Xe;
  Real _HV;
  Real _D0V;
  Real _HV2;
  Real _D0V2;
  Real _HI;
  Real _D0I;
  Real _Z;
  Real _dHVaUI;
  Real _dHVa2;
  Real _dSVa2;
  Real _dHVa;
  Real _dSVa;
  Real _dHUI;
  Real _dHXeVaUI;

  MaterialProperty<Real> & _EBgXeVa;
  MaterialProperty<Real> & _EBgXeVa2;
  //  MaterialProperty<RealGradient> & _grad_EBXeVa;
  MaterialProperty<Real> & _EBgVaVa;
  //  MaterialProperty<RealGradient> & _grad_EBVaVa;

  MaterialProperty<Real> & _SBgXeVa;
  MaterialProperty<Real> & _SBgXeVa2;
  //  MaterialProperty<RealGradient> & _grad_EBXeVa;
  MaterialProperty<Real> & _SBgVaVa;
  //  MaterialProperty<RealGradient> & _grad_EBVaVa;

  MaterialProperty<Real> & _LgXeVa;
  MaterialProperty<Real> & _LgUVa;
  MaterialProperty<Real> & _LgXeU;
  MaterialProperty<Real> & _LgXeVaU;
  MaterialProperty<Real> & _LgXeVaVa;
  MaterialProperty<Real> & _LgXeVaXe;
  MaterialProperty<Real> & _LgXeVa2;
  MaterialProperty<Real> & _LgXeVaVa2;
  MaterialProperty<Real> & _LgUVa2;
  MaterialProperty<Real> & _LgVaVa2;

  MaterialProperty<Real> & _Dg;
  MaterialProperty<Real> & _Mg;


  MaterialProperty<Real> & _Dv;
  MaterialProperty<Real> & _Mv;
  MaterialProperty<RealGradient> & _grad_Mg;
  MaterialProperty<RealGradient> & _grad_Mv;

  MaterialProperty<Real> & _Dv2;
  MaterialProperty<Real> & _Mv2;
  MaterialProperty<RealGradient> & _grad_Mv2;

  MaterialProperty<Real> & _DI;
  MaterialProperty<Real> & _MI;
  MaterialProperty<RealGradient> & _grad_MI;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _Zg;
  MaterialProperty<Real> & _dgHVaUI;
  MaterialProperty<Real> & _dgHVa2;
  MaterialProperty<Real> & _dgSVa2;
  MaterialProperty<Real> & _dgHVa;
  MaterialProperty<Real> & _dgSVa;
  MaterialProperty<Real> & _dgHUI;
  MaterialProperty<Real> & _dgHXeVaUI;
  MaterialProperty<std::vector<Real> > & _vC_i;
  MaterialProperty<std::vector<Real> > & _vm_i;
  MaterialProperty<std::vector<Real> > & _vb_i;
  MaterialProperty<std::vector<Real> > & _vVaC_i;
  MaterialProperty<std::vector<Real> > & _vVam_i;
  MaterialProperty<std::vector<Real> > & _vVab_i;
  MaterialProperty<std::vector<Real> > & _vUIC_i;
  MaterialProperty<std::vector<Real> > & _vUIm_i;
  MaterialProperty<std::vector<Real> > & _vUIb_i;
  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _kappa_cU;
  MaterialProperty<Real> & _kappa_cV;
  MaterialProperty<Real> & _kappa_cGV;
  MaterialProperty<Real> & _kappa_cVV;
  MaterialProperty<Real> & _kappa_cVU;
  MaterialProperty<Real> & _kappa_cGVU;
  MaterialProperty<Real> & _kappa_cVVU;
  MaterialProperty<Real> & _kappa_cUI;

  MaterialProperty<Real> & _kI2;
  MaterialProperty<Real> & _kV2;

  MaterialProperty<Real> & _kappa_GB;
  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _mu;

  MaterialProperty<Real> & _cu;

  MaterialProperty<RealGradient> & _grad_cu;
  //  MaterialProperty<Real> & _cu_old;
  //  MaterialProperty<RealGradient> & _grad_cu_old;

  MaterialProperty<Real> & _LogC;
  MaterialProperty<Real> & _LogC2;
  MaterialProperty<Real> & _LogTol;

  MaterialProperty<Real> & _tgrad_corr_mult;

  unsigned int _n;
  // unsigned int _nVa;
};

#endif //UO2ANISOCONSTEMPXEUVAB_H
