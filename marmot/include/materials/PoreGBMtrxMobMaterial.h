#ifndef POREGBMTRXMOBMATERIAL_H
#define POREGBMTRXMOBMATERIAL_H

#include "Material.h"

//Forward Declarations
class PoreGBMtrxMobMaterial;

template<>
InputParameters validParams<PoreGBMtrxMobMaterial>();

/**
 * Calculated vacancy and interstitial properties for a given material. No defaults.
 * The temperature must be in Kelvin
 */
class PoreGBMtrxMobMaterial : public Material
{
public:
  PoreGBMtrxMobMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _T;
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  VariableValue & _c;
  VariableGradient & _grad_c;

  MaterialProperty<RealTensorValue> & _M_c;
  MaterialProperty<RealTensorValue> & _dMdc;
  MaterialProperty<Real> & _sM;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _l_GB;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _kappa_GB;
  MaterialProperty<Real> & _mu;
  MaterialProperty<Real> & _gamma;
  MaterialProperty<Real> & _tgrad_corr_mult;

  Real _int_width;
  Real _length_scale;
  Real _time_scale;
  Real _D0;
  Real _Em;
  Real _GB_energy;
  Real _GBmob0;
  Real _Q;
  Real _en_ratio;
  Real _GBMobility;
  Real _surfindex;
  Real _gbindex;
  Real _bulkindex;

  const Real _JtoeV;
  const Real _kb;
  unsigned int _ncrys;
};

#endif //POREGBMTRXMOBMATERIAL_H
