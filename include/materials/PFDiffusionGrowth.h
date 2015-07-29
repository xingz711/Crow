#ifndef PFDIFFUSIONGROWTH_H
#define PFDIFFUSIONGROWTH_H

#include "Material.h"

//Forward Declarations
class PFDiffusionGrowth;

template<>
InputParameters validParams<PFDiffusionGrowth>();

class PFDiffusionGrowth : public Material
{
public:
  PFDiffusionGrowth(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  Real _Dvol;
  Real _Dvap;
  Real _Dsurf;
  Real _Dgb;

  Real _kappa;

  VariableValue & _rho;
  VariableGradient & _grad_rho;
  VariableValue & _v;

  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<RealGradient> & _grad_D;
  MaterialProperty<Real> & _dDdc;

  unsigned int _ncrys;
};

#endif //PFDIFFUSIONGROWTH_H
