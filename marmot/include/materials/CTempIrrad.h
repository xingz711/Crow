#ifndef CTEMPIRRAD_H
#define CTEMPIRRAD_H

#include "Material.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class CTempIrrad;

template<>
InputParameters validParams<CTempIrrad>();

/**
 * Calculated vacancy and interstitial properties for UO2 (or other material if different values are passed in).
 * The temperature must be in Kelvin and can come from three different sources, either a single, constant value,
 * a single value from BISON or as a coupled temperature.
 */
class CTempIrrad : public DerivativeMaterialInterface<Material>
{
public:
  CTempIrrad(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _cv;
  VariableName _cv_name;

  bool _coupled_temp;
  const Real * _temp;
  VariableValue & _T;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _M_v;
  MaterialProperty<Real> & _Ms_v;
  MaterialProperty<Real> & _D_v;
  MaterialProperty<Real> & _dM_vdcv;
  MaterialProperty<Real> & _d2M_vdcv2;
  MaterialProperty<Real> & _M_i;

  MaterialProperty<Real> & _Efv;
  MaterialProperty<Real> & _Efi;

  MaterialProperty<Real> & _L_eta;
  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_v;
  MaterialProperty<Real> & _cv_eq;
  MaterialProperty<Real> & _W;

  Real _int_width;
  const Real _length_scale;
  const Real _conv_f;
  Real _time_scale;
  unsigned int _free_energy_form;
  Real _D0vac;
  Real _Emvac;
  Real _Efvac;
  Real _D0int;
  Real _Emint;
  Real _Efint;
  Real _surface_energy;
};

#endif //CTEMPIRRAD_H
