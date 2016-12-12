#ifndef RESISTIVITYMATERIAL_H
#define RESISTIVITYMATERIAL_H

#include "Material.h"

//Forward Declarations
class ResistivityMaterial;

template<>
InputParameters validParams<ResistivityMaterial>();

/**
 * This kernel computes the thermophysical material properties of UO2 and determines the effect of porosity.  Reference found in:
 * J.K. Fink, Thermophysical Properties of Uranium Dioxide, Journal of Nuclear
 * Materials, Vol. 279 p. 1-18, 2000.
 * \see MesoUO2
 */
class ResistivityMaterial : public Material
{
public:
  ResistivityMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  Real _length_scale;
  Real _ref_resis;
  Real _temp_coeff;
  Real _ref_temp;
  const VariableValue & _T;

  std::string _base_name;
  MaterialProperty<Real> & _resistivity;
  MaterialProperty<Real> & _electic_conductivity;
};

#endif //RESISTIVITYMATERIAL_H
