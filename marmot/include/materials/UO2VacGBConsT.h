#ifndef UO2VACGBCONST_H
#define UO2VACGBCONST_H

#include "Material.h"

//Forward Declarations
class UO2VacGBConsT;

template<>
InputParameters validParams<UO2VacGBConsT>();

class UO2VacGBConsT : public Material
{
public:
  UO2VacGBConsT(const InputParameters & parameters);

protected:
  virtual void computeProperties();
  VariableValue & _cv;

private:
  Real _sigma_SS;
  Real _sigma_GB;
  Real _l_SS;
  Real _l_GB;
  Real _temp;
  Real _M0_GB;
  Real _Q;
  Real _Dg_mult;

  MaterialProperty<Real> & _kappa;
  MaterialProperty<Real> & _Dg;
  MaterialProperty<Real> & _Dv;
  MaterialProperty<RealGradient> & _grad_Dv;
  MaterialProperty<Real> & _Mv;
  MaterialProperty<RealGradient> & _grad_Mv;
  MaterialProperty<Real> & _L;
  MaterialProperty<Real> & _Ao;
  MaterialProperty<Real> & _BetaS;
  MaterialProperty<Real> & _BetaGB;
  MaterialProperty<Real> & _mu;

  std::vector<VariableValue *> _vals;

  Real _kb;
  Real _Dv0;
  Real _Evm;
  Real _M_GB;

  unsigned int _n;
};

#endif //UO2VACGBCONST_H
