#ifndef POREGBMATERIAL_H
#define POREGBMATERIAL_H

#include "Material.h"

//Forward Declarations
class PoreGBMaterial;

template<>
InputParameters validParams<PoreGBMaterial>();

/**
 * Calculated vacancy and interstitial properties for a given material. No defaults.
 * The temperature must be in Kelvin.
 */
class PoreGBMaterial : public Material
{
public:
  PoreGBMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _T;

  MaterialProperty<Real> & _M_c;
  MaterialProperty<RealGradient> & _grad_M_c;
  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _L;
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

  const Real _JtoeV;
  const Real _kb;
};

#endif //POREGBMATERIAL_H
