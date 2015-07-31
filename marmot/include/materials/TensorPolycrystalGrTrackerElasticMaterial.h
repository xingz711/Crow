// Original class author: A.M. Jokisaari,  O. Heinonen

#ifndef TENSORPOLYCRYSTALGRTRACKERELASTICMATERIAL_H
#define TENSORPOLYCRYSTALGRTRACKERELASTICMATERIAL_H

#include "LinearElasticMaterial.h"

//Forward Declarations
class TensorPolycrystalGrTrackerElasticMaterial;
class GrainTracker;

/**
 * TensorPolycrystalGrTrackerElasticMaterial handles a fully anisotropic, single-crystal material's elastic
 * constants.  It takes all 21 independent stiffness tensor inputs, or only 9, depending on the
 * boolean input value given.  This can be extended or simplified to specify HCP, monoclinic,
 * cubic, etc as needed.
 */
class TensorPolycrystalGrTrackerElasticMaterial : public LinearElasticMaterial
{
public:
  TensorPolycrystalGrTrackerElasticMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpElasticityTensor();
  Real _length_scale;
  Real _pressure_scale;

  std::string _Euler_angles_file_name;
  const GrainTracker & _grain_tracker;
  unsigned int _grain_num;
  unsigned int _stiffness_buffer;

  std::vector<VariableValue *> _vals;

  std::vector< MaterialProperty<ElasticityTensorR4> *> _D_elastic_tensor;
  std::vector<ElasticityTensorR4> _C_rotated;

  const Real _JtoeV;
  unsigned int _nop;
};

#endif //TENSORPOLYCRYSTALGRTRACKERELASTICMATERIAL_H
