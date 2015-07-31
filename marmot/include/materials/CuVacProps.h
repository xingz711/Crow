#ifndef CUVACPROPS_H
#define CUVACPROPS_H

#include "Material.h"

//Forward Declarations
class CuVacProps;

template<>
InputParameters validParams<CuVacProps>();

class CuVacProps : public Material
{
public:
  CuVacProps(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _cv;
  VariableGradient & _grad_cv;

  std::string _coupled_temp_name;
  VariableValue & _T;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _M_v;
  MaterialProperty<Real> & _Ms_v;
  MaterialProperty<Real> & _D_v;
  MaterialProperty<RealGradient> & _grad_M_v;
  MaterialProperty<Real> & _M_i;
  MaterialProperty<RealGradient> & _grad_M_i;

  MaterialProperty<Real> & _Efv;
  MaterialProperty<Real> & _Efi;

  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_v;
  MaterialProperty<Real> & _cv_eq;
  MaterialProperty<Real> & _W;
  MaterialProperty<Real> & _Qstar;

  Real _int_width;
  Real _length_scale;
  Real _time_scale;
  unsigned int _free_energy_form;
  Real _en_ratio;
  Real _Dv0in, _Emvin, _Efvin;
};

#endif //CUVACPROPS_H
