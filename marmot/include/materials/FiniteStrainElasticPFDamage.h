#ifndef FINITESTRAINELASTICPFDAMAGE_H
#define FINITESTRAINELASTICPFDAMAGE_H

#include "FiniteStrainMaterial.h"
#include "DerivativeMaterialInterface.h"
#include "Function.h"

/**
 * Calculate strain energy available for crack growth.
 * Finite strain Elastic formulation.
 * Void is treated separately, scales the stiffness matrix.
 * Read from file (slow) or function using bilinear interpolation (fast).
 */
class FiniteStrainElasticPFDamage : public DerivativeMaterialInterface<FiniteStrainMaterial>
{
public:
  FiniteStrainElasticPFDamage(const InputParameters & parameters);

protected:
  virtual void computeQpStress();
  virtual void computeQpElasticityTensor();
  virtual void update_var(RankTwoTensor &a);
  virtual void initQpStatefulProperties();
  virtual void read_prop();

  VariableValue & _c;
  VariableName _c_name;

  MaterialProperty<ElasticityTensorR4> & _delasticity_tensor_dc;
  MaterialProperty<ElasticityTensorR4> & _d2elasticity_tensor_dc2;
  MaterialProperty<RankTwoTensor> & _dstress_dc;

  MaterialProperty<Real> & _G0_pos;
  MaterialProperty<Real> & _G0_pos_old;
  MaterialProperty<Real> & _G0_neg;

  MaterialProperty<RankTwoTensor> &_stress0;
  MaterialProperty<RankTwoTensor> &_stress0_old;

  MaterialProperty<Real> &_d_void;
  MaterialProperty<Real> &_d_void_old;

  bool _has_function;
  Function * _function;

  Real _kvoid;

  FileName _void_prop_file_name;
};

#endif //FINITESTRAINELASTICPFDAMAGE_H
