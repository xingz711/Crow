#ifndef PFDIFFUSIONGROWTHMTRX_H
#define PFDIFFUSIONGROWTHMTRX_H

#include "Material.h"

//Forward Declarations
class PFDiffusionGrowthMtrx;

template<>
InputParameters validParams<PFDiffusionGrowthMtrx>();

class PFDiffusionGrowthMtrx : public Material
{
public:
  PFDiffusionGrowthMtrx(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  std::vector<const VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;
  std::vector<const VariableGradient *> _grad_vals;

  Real _Dvol;
  Real _Dvap;
  Real _Dsurf;
  Real _Dgb;

  Real _kappa;

  const VariableValue & _rho;
  const VariableGradient & _grad_rho;
  const VariableValue & _v;

  MaterialProperty<RealTensorValue> & _D;
  // MaterialProperty<Real> & _kappa_c;
  MaterialProperty<RealTensorValue> & _dDdc;

  unsigned int _ncrys;
};

#endif //PFDIFFUSIONGROWTHMTRX_H
