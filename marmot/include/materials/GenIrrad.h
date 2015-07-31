#ifndef GENIRRAD_H
#define GENIRRAD_H

#include "Material.h"

//Forward Declarations
class GenIrrad;

template<>
InputParameters validParams<GenIrrad>();

class GenIrrad : public Material
{
public:
  GenIrrad(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _cv;
  VariableGradient & _grad_cv;
  VariableValue & _ci;
  VariableGradient & _grad_ci;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _M_v;
  MaterialProperty<RealGradient> & _grad_M_v;
  MaterialProperty<Real> & _M_i;
  MaterialProperty<RealGradient> & _grad_M_i;

  MaterialProperty<Real> & _Efv;
  MaterialProperty<Real> & _Efi;

  MaterialProperty<Real> & _L;

  MaterialProperty<Real> & _cv_eq;

  MaterialProperty<Real> & _kappa_op;
  MaterialProperty<Real> & _kappa_v;
  MaterialProperty<Real> & _kappa_i;

  Real _kappa_eta;
  Real _kappa_cv;
  Real _kappa_ci;
  Real _T;
  Real _Diff;
  Real _w;
  Real _a;
  MooseEnum _free_energy_form;

  const Real _kb; // Boltzmann constant
};

#endif //GENIRRAD_H
