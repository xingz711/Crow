#ifndef PFDIFFUSIONGROWTHCONST_H
#define PFDIFFUSIONGROWTHCONST_H

#include "Material.h"

//Forward Declarations
class PFDiffusionGrowthConst;

template<>
InputParameters validParams<PFDiffusionGrowthConst>();

class PFDiffusionGrowthConst : public Material
{
public:
  PFDiffusionGrowthConst(const std::string & name,
             InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  //std::vector<VariableValue *> _vals;
  //std::vector<unsigned int> _vals_var;

  Real _Dvol;
  Real _Dvap;
  Real _Dsurf;
  Real _Dgb;
  
  Real _beta;
  Real _kappa;
  Real _l;

  //VariableValue & _T;
  VariableValue & _rho;
  VariableGradient & _grad_rho;
  //VariableValue & _eta;

  MaterialProperty<Real> & _D;
  MaterialProperty<Real> & _kappa_op;
  MaterialProperty<Real> & _kappa_c;
  MaterialProperty<Real> & _L;
  MaterialProperty<RealGradient> & _grad_D;
  //unsigned int _ncrys;

};

#endif //PFDIFFUSIONGROWTHCONST_H



