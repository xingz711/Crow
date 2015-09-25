#ifndef SINTERINGDIFFUSIONGROWTH_H
#define SINTERINGDIFFUSIONGROWTH_H

#include "Material.h"

//Forward Declarations
class SinteringDiffusionGrowth;

template<>
InputParameters validParams<SinteringDiffusionGrowth>();

class SinteringDiffusionGrowth : public Material
{
public:
  SinteringDiffusionGrowth(const InputParameters & parameters);

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
  Real _GB_energy;
  Real _surface_energy;
  Real _GBmob0;
  Real _Qbulk;
  Real _Qmgb;
  Real _Qs;
  Real _Qgb;

  Real _kappa;

  VariableValue & _T;
  VariableValue & _rho;
  VariableValue & _v;

  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _dDdc;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _kappa_op;

  const Real _kb;
  unsigned int _ncrys;
};

#endif //SINTERINGDIFFUSIONGROWTH_H
