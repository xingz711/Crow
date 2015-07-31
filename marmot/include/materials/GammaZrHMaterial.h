#ifndef GAMMAZRHMATERIAL_H
#define GAMMAZRHMATERIAL_H

#include "Material.h"

//Forward Declarations
class GammaZrHMaterial;

/// \todo This needs to use default coupling
template<>
InputParameters validParams<GammaZrHMaterial>();

/**
 * Mobilities for the Gamma Zirconium Hydride phase.
 */
class GammaZrHMaterial : public Material
{
public:
  GammaZrHMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  VariableValue & _T;

  Real _length_scale;
  Real _time_scale;

  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _M;
  MaterialProperty<RealGradient> & _grad_M;
  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _beta;

  const Real _C_m;
  const Real _C_gamma;
  MaterialProperty<Real> & _G;
};

#endif //GAMMAZRHMATERIAL_H
