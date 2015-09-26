#ifndef SINTERINGDIFFUSION_H
#define SINTERINGDIFFUSION_H

#include "Material.h"

//Forward Declarations
class SinteringDiffusion;

template<>
InputParameters validParams<SinteringDiffusion>();

class SinteringDiffusion : public Material
{
public:
  SinteringDiffusion(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  std::vector<VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  Real _Dvol0;
  Real _Dvap0;
  Real _Dsurf0;
  Real _Dgb0;
  Real _int_width;
  Real _Em;
  Real _GB_energy;
  Real _surface_energy;
  Real _GBmob0;
  Real _Q;
  Real _Qs;
  Real _Qgb;
  const Real _kb;

  VariableValue & _rho;
  VariableGradient & _grad_rho;
  VariableValue & _T;

  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _dDdc;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _kappa_op;

  unsigned int _ncrys;
};

#endif //SinteringDiffusion_H
