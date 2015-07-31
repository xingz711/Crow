#ifndef GENERALGRBU_H
#define GENERALGRBU_H

#include "Material.h"

//Forward Declarations
class GeneralGrBu;

template<>
InputParameters validParams<GeneralGrBu>();

class GeneralGrBu : public Material
{
public:
  GeneralGrBu(const InputParameters & parameters);

protected:
  virtual void computeProperties();
  VariableValue & _cg;
  VariableGradient & _grad_cg;

private:
  Real _kappSS_input;
  Real _kappGB_input;
  Real _Dg_input;
  Real _L_input;
  Real _Ao_input;
  Real _BetaS_input;
  Real _BetaGB_input;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _kappaSS;
  MaterialProperty<Real> & _Dg;
  MaterialProperty<Real> & _Dg2;
  MaterialProperty<RealGradient> & _grad_Dg;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _Ao;
  MaterialProperty<Real> & _BetaS;
  MaterialProperty<Real> & _BetaGB;
  MaterialProperty<Real> & _mu;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
};

#endif //GENERALGRBU_H
