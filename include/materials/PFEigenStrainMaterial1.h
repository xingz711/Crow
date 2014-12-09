#ifndef PFEIGENSTRAINMATERIAL1_H
#define PFEIGENSTRAINMATERIAL1_H

#include "EigenStrainBaseMaterial.h"

/**
 * EigenStrainBaseMaterial is a base to construct material kernels that represent
 * different eigenstrain - concentration relationships.
 * It provides place holders for calculating eigenstrain and its 1st and 2nd
 * order derivatives with respect to c, elasticity_tensor and its 1st and 2nd
 * order derivatives wrt c if it is a function of c instead of a constant.
 */
class PFEigenStrainMaterial1 : public EigenStrainBaseMaterial
{
public:
  PFEigenStrainMaterial1(const std:: string & name, InputParameters parameters);

protected:
  virtual void computeEigenStrain();
     
  VariableValue & _c;
  
  std::vector<VariableValue *> _vals;
  unsigned int _ncrys;
   
private: 
  //VariableValue & _v;
  Real _e_c;
  RankTwoTensor _e_cI;
  Real _e_v;
  RankTwoTensor _e_vI;
};

#endif //PFEIGENSTRAINMATERIAL_H
