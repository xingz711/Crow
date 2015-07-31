#ifndef ACGRGRGENERICDF_H
#define ACGRGRGENERICDF_H

#include "ACBulk.h"


//Forward Declarations
class ACGrGrGenericDF;

template<>
InputParameters validParams<ACGrGrGenericDF>();

/**
 * This kernel add a generic driving force that moves the GB in a specific direction
 */
class ACGrGrGenericDF : public ACBulk
{
public:
  ACGrGrGenericDF(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);

private:
  RealGradient _DF;
  Real _length_scale;

  VariableValue & _c;
  const MaterialProperty<Real> & _l_GB;
  Real _JtoeV;
};

#endif //ACGRGRGENERICDF_H
