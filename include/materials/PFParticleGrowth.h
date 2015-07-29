#ifndef PFPARTICLEGROWTH_H
#define PFPARTICLEGROWTH_H

#include "Material.h"

//Forward Declarations
class PFParticleGrowth;

template<>
InputParameters validParams<PFParticleGrowth>();

class PFParticleGrowth : public Material
{
public:
  PFParticleGrowth(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:

  Real _beta;
  Real _l;

  MaterialProperty<Real> & _kappa_op;
  MaterialProperty<Real> & _L;

};

#endif //PFPARTICLEGROWTH_H
