#ifndef POREGBPURDUEMATERIAL_H
#define POREGBPURDUEMATERIAL_H

#include "Material.h"

//Forward Declarations
class PoreGBPurdueMaterial;

template<>
InputParameters validParams<PoreGBPurdueMaterial>();

/**
 * Calculated vacancy and interstitial properties for a given material.  No defaults.
 * The temperature must be in Kelvin
 */
class PoreGBPurdueMaterial : public Material
{
public:
  PoreGBPurdueMaterial(const std::string & name,
          InputParameters parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _T;
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  VariableValue & _c;
  VariableGradient & _grad_c;

  MaterialProperty<RealTensorValue> & _M;
  MaterialProperty<RealTensorValue> & _dMdc;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _kappa_GB;
  MaterialProperty<Real> & _A;
  MaterialProperty<Real> & _B;
  MaterialProperty<Real> & _time_scale;
  MaterialProperty<Real> & _energy_scale;
  MaterialProperty<Real> & _detM;

  Real _int_width;
  Real _length_scale;
  Real _ls;
  Real _D0;
  Real _Em;
  Real _GB_energy;
  Real _surface_energy;
  Real _GBmob0;
  Real _Q;
  Real _omega;
  Real _Ds0;
  Real _Dgb0;
  Real _Qs;
  Real _Qgb;
  Real _surfindex;
  Real _gbindex;
  Real _bulkindex;
  Real _GBMobility;

  // Constants
  const Real _JtoeV;
  const Real _kb;
  unsigned int _ncrys;
};

#endif //POREGBPURDUEMATERIAL_H
