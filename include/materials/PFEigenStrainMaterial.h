#ifndef PFEIGENSTRAINMATERIAL_H
#define PFEIGENSTRAINMATERIAL_H

#include "EigenStrainBaseMaterial.h"

/**
 * EigenStrainBaseMaterial is a base to construct material kernels that represent
 * different eigenstrain - concentration relationships.
 * It provides place holders for calculating eigenstrain and its 1st and 2nd
 * order derivatives with respect to c, elasticity_tensor and its 1st and 2nd
 * order derivatives wrt c if it is a function of c instead of a constant.
 */
class PFEigenStrainMaterial : public EigenStrainBaseMaterial
{
public:
  PFEigenStrainMaterial(const std:: string & name, InputParameters parameters);

protected:
  virtual void computeEigenStrain();
  //virtual RankTwoTensor computeStressFreeStrain();

  //MaterialProperty<RankTwoTensor> & _eigenstrain;
  //MaterialProperty<RankTwoTensor> & _deigenstrain_dc;
  //MaterialProperty<RankTwoTensor> & _d2eigenstrain_dc2;

  //MaterialProperty<ElasticityTensorR4> & _delasticity_tensor_dc;
  //MaterialProperty<ElasticityTensorR4> & _d2elasticity_tensor_dc2;

  //VariableValue & _c;
  
private: 
  VariableValue & _v;
  Real _e_c;
  RankTwoTensor _e_cI;
  Real _e_v;
  RankTwoTensor _e_vI;
};

#endif //PFEIGENSTRAINMATERIAL_H
