#ifndef MICROTHERMAL_H
#define MICROTHERMAL_H

#include "Material.h"

//Forward Declarations
class MicroThermal;

template<>
InputParameters validParams<MicroThermal>();

/**
 * This kernel computes the thermophysical material properties of UO2 and determines the effect of porosity.  Reference found in:
 * J.K. Fink, Thermophysical Properties of Uranium Dioxide, Journal of Nuclear
 * Materials, Vol. 279 p. 1-18, 2000.
 * \see MesoUO2
 */
class MicroThermal : public Material
{
public:
  MicroThermal(const std::string & name,
          InputParameters parameters);

protected:
  virtual void computeProperties();
  virtual Real computeBulkThermalConductivity();

private:
  bool _has_v;
  Real _length_scale;
  Real _Gas_conductivity;
  Real _GB_cond;
  Real _ko;

  unsigned int _n;
  std::vector<VariableValue *> _vals;

  VariableValue & _eta;

  MaterialProperty<Real> & _thermal_conductivity;
};

#endif //MICROTHERMAL_H
