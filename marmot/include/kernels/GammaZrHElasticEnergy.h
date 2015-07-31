#ifndef GAMMAZRHELASTICENERGY_H
#define GAMMAZRHELASTICENERGY_H

#include "ACBulk.h"
#include "RankTwoTensor.h"
#include "TensorMechanicsMaterial.h"

//Forward Declarations
class GammaZrHElasticEnergy;

template<>
InputParameters validParams<GammaZrHElasticEnergy>();

class GammaZrHElasticEnergy : public ACBulk
{
public:
  GammaZrHElasticEnergy(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

private:
  VariableValue & _eta2;
  VariableValue & _eta3;
  VariableValue & _C;

  std::vector<Real> _trans1_input;
  std::vector<Real> _trans2_input;
  std::vector<Real> _trans3_input;
  const MaterialProperty<ElasticityTensorR4> & _C_ijkl;
  const MaterialProperty<RankTwoTensor> & _elastic_strain;

  RankTwoTensor _eps1;
  RankTwoTensor _eps2;
  RankTwoTensor _eps3;
};

#endif //GAMMAZRHELASTICENERGY_H
