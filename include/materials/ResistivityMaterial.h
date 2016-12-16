#ifndef RESISTIVITYMATERIAL_H
#define RESISTIVITYMATERIAL_H

#include "Material.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class ResistivityMaterial;
// class DerivativeMaterialInterface;

template<>
InputParameters validParams<ResistivityMaterial>();

/**
 * This kernel computes the thermophysical material properties of UO2 and determines the effect of porosity.  Reference found in:
 * J.K. Fink, Thermophysical Properties of Uranium Dioxide, Journal of Nuclear
 * Materials, Vol. 279 p. 1-18, 2000.
 * \see MesoUO2
 */
class ResistivityMaterial : public DerivativeMaterialInterface<Material>
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
  MaterialProperty<Real> & _dresistivity_dT;
  MaterialProperty<Real> & _electric_conductivity;
  MaterialProperty<Real> & _delectric_conductivity_dT;
};

#endif //RESISTIVITYMATERIAL_H
