#ifndef EIGENSTRAINFRACMATERIAL_H
#define EIGENSTRAINFRACMATERIAL_H

#include "EigenStrainBaseMaterial.h"


class EigenStrainFracMaterial : public EigenStrainBaseMaterial
{
public:
  EigenStrainFracMaterial(const InputParameters & parameters);

protected:
  virtual void computeEigenStrain();
  virtual void computeQpElasticityTensor();

  MaterialProperty<RankFourTensor> & _elasticity_tensor0;
  MaterialProperty<Real> & _G;
  MaterialProperty<Real> & _G0;
};

#endif //EIGENSTRAINFRACMATERIAL_H
