#ifndef PFTEMPMOBILITY_H
#define PFTEMPMOBILITY_H

#include "Material.h"

//Forward Declarations
class PFTempMobility;

template<>
InputParameters validParams<PFTempMobility>();

class PFTempMobility : public Material
{
public:
  PFTempMobility(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  //std::vector<VariableValue *> _vals;
  //std::vector<unsigned int> _vals_var;

  Real _Da;
  Real _Db;
  Real _R;
  Real _Vm;

  //Real _beta;
  Real _kappa;
  //Real _l;

  const VariableValue & _c;
  const VariableGradient & _grad_c;
  const VariableValue & _T;

  MaterialProperty<Real> & _M;
  //MaterialProperty<Real> & _kappa_op;
  MaterialProperty<Real> & _kappa_c;
  //MaterialProperty<Real> & _L;
  MaterialProperty<RealGradient> & _grad_M;
  //unsigned int _ncrys;

};

#endif //PFDIFFUSION_H
