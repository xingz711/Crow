#ifndef SPLITCHWRESGBSURFDIFF_H
#define SPLITCHWRESGBSURFDIFF_H

#include "Kernel.h"

//Forward Declarations
class SplitCHWResGBSurfDiff;

template<>
InputParameters validParams<SplitCHWResGBSurfDiff>();

/**
 * This kernel computes the residual of the substitutional variable in the split cahn-hilliard formulation.
 * In addition, it includes the effects of GB and surface diffusion, if desired.
 */
class SplitCHWResGBSurfDiff : public Kernel
{
public:
  SplitCHWResGBSurfDiff(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::vector<VariableValue *> _vals;
  std::string _mob_name;
  Real _diffindex;
  Real _gbindex;
  Real _bulkindex;
  unsigned int _c_var;
  MooseArray<Real> & _c;
  const MaterialProperty<Real> & _mobtemp;
  unsigned int _ncrys;
};

#endif //SPLITCHWRESGBSURFDIFF_H
