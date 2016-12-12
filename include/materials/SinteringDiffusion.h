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
  virtual void computeProperties();

private:
  std::vector<const VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  const Real _Dvol0;
  const Real _Dvap0;
  const Real _Dsurf0;
  const Real _Dgb0;
  const Real _int_width;
  const Real _length_scale;
  const Real _time_scale;
  const Real _Qv;
  const Real _GB_energy;
  const Real _surface_energy;
  const Real _GBmob0;
  const Real _Q;
  const Real _Qs;
  const Real _Qgb;
  const Real _Vm;
  const Real _GBMobility;
  const Real _kb;
  const Real _JtoeV;

  const VariableValue & _c;
  const VariableGradient & _grad_c;
  const VariableValue & _T;

  MaterialProperty<Real> & _D;
  // MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _dDdc;
  MaterialProperty<Real> & _M;
  MaterialProperty<Real> & _dMdc;
  MaterialProperty<Real> & _L;
  // MaterialProperty<Real> & _kappa_op;
  // MaterialProperty<Real> & _A;
  // MaterialProperty<Real> & _B;
  const MaterialProperty<Real> & _A;
  const MaterialProperty<Real> & _B;

  unsigned int _op_num;
};

#endif //SinteringDiffusion_H
