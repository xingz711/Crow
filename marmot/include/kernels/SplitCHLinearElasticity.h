#ifndef SPLITCHLINEARELASTICITY_H
#define SPLITCHLINEARELASTICITY_H

#include "SplitCHBase.h"
#include "RankTwoTensor.h"
#include "ElasticityTensorR4.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class SplitCHLinearElasticity;

template<>
InputParameters validParams<SplitCHLinearElasticity>();

class SplitCHLinearElasticity : public DerivativeMaterialInterface<SplitCHBase>
{
public:
  SplitCHLinearElasticity(const InputParameters & parameters);

protected:
  virtual Real computeDEDC(PFFunctionType type) ;

private:
  /// Material property base name to allow for multiple TensorMechanicsMaterial to coexist in the same simulation
  std::string _base_name;
  std::string _elastic_strain_name;
  std::string _elasticity_tensor_name;

  const MaterialProperty<RankTwoTensor> & _stress;

  const MaterialProperty<RankTwoTensor> & _elastic_strain;
  const MaterialProperty<RankTwoTensor> & _delastic_strain_dc;
  const MaterialProperty<RankTwoTensor> & _d2elastic_strain_dc2;

  const MaterialProperty<ElasticityTensorR4> & _elasticity_tensor;
  const MaterialProperty<ElasticityTensorR4> & _delasticity_tensor_dc;
  const MaterialProperty<ElasticityTensorR4> & _d2elasticity_tensor_dc2;
};

#endif //SPLITCHLINEARELASTICITY_H
