#ifndef TEMPDIFFUSION_H
#define TEMPDIFFUSION_H

#include "Material.h"

//Forward Declarations
class TempDiffusion;

template<>
InputParameters validParams<TempDiffusion>();

class TempDiffusion : public Material
{
public:
  TempDiffusion(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

private:
  //std::vector<VariableValue *> _vals;
  //std::vector<unsigned int> _vals_var;

  Real _Dv;
  Real _kb;
  Real _T;
  Real _kappa;

  VariableValue & _c;
  VariableGradient & _grad_c;

  MaterialProperty<Real> & _D;
  MaterialProperty<RealGradient> & _grad_D;
  MaterialProperty<Real> & _kappa_c;

};

#endif //TempDIFFUSION_H
