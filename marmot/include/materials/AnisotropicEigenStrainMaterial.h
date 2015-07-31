#ifndef ANISOTROPICEIGENSTRAINMATERIAL_H
#define ANISOTROPICEIGENSTRAINMATERIAL_H

#include "EigenStrainBaseMaterial.h"
#include "ElasticityTensorR4.h"

/**
 * AnisotropicEigenStrainMaterial assumes a nonlinear relationship between eigenstrain and concentration c as eigenstrain = -epsilon * c * ( 1.0 - c).
 */
class AnisotropicEigenStrainMaterial : public EigenStrainBaseMaterial
{
public:
  AnisotropicEigenStrainMaterial(const InputParameters & parameters);

protected:
  virtual void computeEigenStrain();
  virtual void computeQpElasticityTensor();

  Real _length_scale;
  Real _pressure_scale;
  std::vector<Real> _eigenstrain_coef;
  Real _factor;
  bool _inhomogeneity;
};

#endif //ANISOTROPICEIGENSTRAINMATERIAL_H
