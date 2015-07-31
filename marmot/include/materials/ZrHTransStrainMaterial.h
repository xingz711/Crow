#ifndef ZRHTRANSSTRAINMATERIAL_H
#define ZRHTRANSSTRAINMATERIAL_H

#include "EigenStrainBaseMaterial.h"

/**
 * UNDOCUMENTED
 */
class ZrHTransStrainMaterial : public EigenStrainBaseMaterial
{
public:
  ZrHTransStrainMaterial(const InputParameters & parameters);

protected:
  virtual void computeEigenStrain();

  VariableValue & _eta1;
  VariableValue & _eta2;
  VariableValue & _eta3;

  MaterialProperty<RankTwoTensor> & _trans1;
  MaterialProperty<RankTwoTensor> & _trans2;
  MaterialProperty<RankTwoTensor> & _trans3;
};

#endif //ZRHTRANSSTRAINMATERIAL_H
