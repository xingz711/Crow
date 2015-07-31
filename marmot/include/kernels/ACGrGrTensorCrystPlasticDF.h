#ifndef ACGRGRTENSORCRYSTPLASTICDF_H
#define ACGRGRTENSORCRYSTPLASTICDF_H

#include "ACBulk.h"
#include <string>

//Forward Declarations
class ACGrGrTensorCrystPlasticDF;

template<>
InputParameters validParams<ACGrGrTensorCrystPlasticDF>();

/**
 * Calculates the porton of the Allen-Cahn equation that results from both the elastic and plastic deformation energy.\
 * FIXME: this kernel is untested!
 */
class ACGrGrTensorCrystPlasticDF : public ACBulk
{
public:
  ACGrGrTensorCrystPlasticDF(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

private:
  std::string _elastic_name;
  std::string _plastic_name;

  const MaterialProperty<Real> & _elastic_driving_force;
  const MaterialProperty<Real> & _plastic_driving_force;
  bool _elastic_force;
  bool _plastic_force;

};
#endif //ACGRGRTENSORCRYSTPLASTICDF_H
