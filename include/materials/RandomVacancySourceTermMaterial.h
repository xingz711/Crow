#ifndef RANDOMVACANCYSOURCETERMMATERIAL_H
#define RANDOMVACANCYSOURCETERMMATERIAL_H

#include "Material.h"

//Forward Declarations
class RandomVacancySourceTermMaterial;

template<>
InputParameters validParams<RandomVacancySourceTermMaterial>();


class RandomVacancySourceTermMaterial : public Material
{
public:
  RandomVacancySourceTermMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeQpProperties();
 // virtual Real computeQpJacobian();

private:
  Real _min;
  Real _max;
  Real _range;
  
  MaterialProperty<Real> & _R;
};

#endif //VACANCYSOURCETERMKERNEL_H
