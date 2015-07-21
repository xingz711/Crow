#ifndef PFDiffusionGrowth1_H
#define PFDiffusionGrowth1_H

#include "Material.h"

//Forward Declarations
class PFDiffusionGrowth1;

template<>
InputParameters validParams<PFDiffusionGrowth1>();

class PFDiffusionGrowth1 : public Material
{
public:
  PFDiffusionGrowth1(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:

  Real _Dvol;
  Real _Dvap;
  Real _Dsurf;
  Real _Dgb;

  Real _beta;
  Real _kappa;
  Real _l;

  VariableValue & _c;
  VariableGradient & _grad_c;
  VariableValue & _sumetaj;

  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _kappa_op;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _L;
  MaterialProperty<RealGradient> & _grad_D;
  unsigned int _ncrys;

};

#endif //PFDiffusionGrowth_H
