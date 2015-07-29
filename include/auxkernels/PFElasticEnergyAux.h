#ifndef PFELASTICENERGYAUX_H
#define PFELASTICENERGYAUX_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class PFElasticEnergyAux;
class SymmTensor;

template<>
InputParameters validParams<PFElasticEnergyAux>();


class PFElasticEnergyAux : public AuxKernel
{
public:
  PFElasticEnergyAux( const InputParameters & parameters );

  virtual ~PFElasticEnergyAux() {}

protected:
  virtual Real computeValue();

  const MaterialProperty<SymmTensor> & _stress;
  const MaterialProperty<SymmTensor> & _elastic_strain;

//private:
  //std::vector<Real> _applied_stress_vector;
  //SymmTensor _applied_stress_tensor;

};

#endif // PFELASTICENERGYAUX_H
