#ifndef CHRADSINKANISOUI_H
#define CHRADSINKANISOUI_H

#include "Kernel.h"

//Forward Declarations
class CHRadSinkAnisoUI;

template<>
InputParameters validParams<CHRadSinkAnisoUI>();

class CHRadSinkAnisoUI : public Kernel
{
public:
  CHRadSinkAnisoUI(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  std::vector<VariableValue *> _vals;
  std::vector<VariableGradient *> _grad_vals;

  const MaterialProperty<std::vector<Real> > & _vUIC_i;
  const MaterialProperty<std::vector<Real> > & _vUIm_i;
  const MaterialProperty<std::vector<Real> > & _vUIb_i;

  const MaterialProperty<Real> & _M;

  const MaterialProperty<Real> & _kb;
  const MaterialProperty<Real> & _kT;

  const MaterialProperty<Real> & _LogC;
  const MaterialProperty<Real> & _LogTol;

  unsigned int _n;
};

#endif //CHRADSINKANISOUI_H
