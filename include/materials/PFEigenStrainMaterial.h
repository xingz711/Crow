#ifndef PFEIGENSTRAINMATERIAL_H
#define PFEIGENSTRAINMATERIAL_H

#include "PFEigenStrainBaseMaterial.h"

/**
 * EigenStrainBaseMaterial is a base to construct material kernels that represent
 * different eigenstrain - concentration relationships.
 * It provides place holders for calculating eigenstrain and its 1st and 2nd
 * order derivatives with respect to c, elasticity_tensor and its 1st and 2nd
 * order derivatives wrt c if it is a function of c instead of a constant.
 */
class PFEigenStrainMaterial : public PFEigenStrainBaseMaterial
{
public:
  PFEigenStrainMaterial(const std:: string & name, InputParameters parameters);

protected:
  virtual void computeEigenStrain();
  virtual void computeQpElasticityTensor();
      
private: 
    Real _epsilon0;
    Real _c0;
    std::vector<Real> _e_v;
};

#endif //PFEIGENSTRAINMATERIAL1_H
