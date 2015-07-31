#ifndef MESOUO2_H
#define MESOUO2_H

#include "Material.h"

//Forward Declarations
class MesoUO2;

template<>
InputParameters validParams<MesoUO2>();

/**
 * This kernel computes the thermophysical material properties of UO2 and determines the effect of porosity.  Reference found in:
 * J.K. Fink, Thermophysical Properties of Uranium Dioxide, Journal of Nuclear
 * Materials, Vol. 279 p. 1-18, 2000.
 * \see MicroThermal
 */
class MesoUO2 : public Material
{
public:
  MesoUO2(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  bool _has_v;
  Real _length_scale;
  bool _LANL_vals;
  Real _Gas_conductivity;
  unsigned int _GBtype;
  Real _GB_width;
  int _kstep;
  Real _GB_cond;
  Real _ko;

  unsigned int _n;

  std::vector<VariableValue *> _vals;

  VariableValue & _temp;
  VariableValue & _eta;

  MaterialProperty<Real> & _thermal_conductivity;
};

#endif //MESOUO2_H
