#ifndef MOBILITY_H
#define MOBILITY_H

#include "Material.h"

//Forward Declarations
class Mobility;

template<>
InputParameters validParams<Mobility>();

/**
 * Calculated properties for a single component phase field model using polynomial free energies
 */
class Mobility : public Material
{
public:
  Mobility(const std::string & name,
                         InputParameters parameters);

protected:
  virtual void computeQpProperties();

  ///Variable values
  VariableValue & _c;
  VariableName _c_name;
  VariableValue & _T;

  ///Mateiral property declarations
  MaterialProperty<Real> & _M;
  MaterialProperty<RealGradient> & _grad_M;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _c_eq;
  MaterialProperty<Real> & _Qstar;
  MaterialProperty<Real> & _D;

  ///Input parameters
  Real _int_width;
  Real _length_scale;
  Real _time_scale;
  Real _D0;
  Real _Ef;
  Real _surface_energy;

  const Real _JtoeV;
  const Real _kb;
};

#endif //MOBILITY_H
