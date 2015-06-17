#ifndef FORCEDENSITYMATERIAL_H
#define FORCEDENSITYMATERIAL_H

#include "Material.h"

//Forward Declarations
class ForceDensityMaterial;

template<>
InputParameters validParams<ForceDensityMaterial>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class ForceDensityMaterial : public Material
{
public:
  ForceDensityMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeQpProperties();

private:
  VariableValue & _c;

  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;
  std::vector<unsigned int> _vals_var;
  std::vector<Real> _product_etas;
  std::vector<RealGradient> _diff_grad_etas;


  Real _ceq;
  Real _cgb;
  Real _k;

  unsigned int _nargs;

  std::vector<MaterialProperty<RealGradient> *> _dF;


  //VariableValue & _c;
};

#endif //FORCEDENSITYMATERIAL_H
