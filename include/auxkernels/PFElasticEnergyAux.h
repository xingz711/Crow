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
  PFElasticEnergyAux( const std::string & name, InputParameters parameters );

  virtual ~PFElasticEnergyAux() {}

protected:
  virtual Real computeValue();

  MaterialProperty<SymmTensor> & _stress;
  MaterialProperty<SymmTensor> & _elastic_strain;
  
//private:
  //std::vector<Real> _applied_stress_vector;
  //SymmTensor _applied_stress_tensor;

};

#endif // PFELASTICENERGYAUX_H
