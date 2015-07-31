#ifndef UO2VACGAS_H
#define UO2VACGAS_H

#include "Material.h"
#include "DerivativeMaterialInterface.h"

//Forward Declarations
class UO2VacGas;

template<>
InputParameters validParams<UO2VacGas>();

/**
 * Properties for the FMM challenge problem using UO2.
 */
class UO2VacGas : public DerivativeMaterialInterface<Material>
{
public:
  UO2VacGas(const InputParameters & parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _cv;
  VariableName _cv_name;
  VariableValue & _cg;
  VariableName _cg_name;

  const Real & _input_temp;
  const Real & _temp;

  MaterialProperty<Real> & _kT;
  MaterialProperty<Real> & _RT;
  MaterialProperty<Real> & _M_v;
  MaterialProperty<Real> & _dM_vdcv;
  MaterialProperty<Real> & _dM_vdcg;
  MaterialProperty<Real> & _d2M_vdcv2;
  MaterialProperty<Real> & _d2M_vdcvdcg;
  MaterialProperty<Real> & _Ms_v;
  MaterialProperty<Real> & _D_v;
  MaterialProperty<Real> & _M_g;
  MaterialProperty<Real> & _dM_gdcg;
  MaterialProperty<Real> & _dM_gdcv;
  MaterialProperty<Real> & _d2M_gdcg2;
  MaterialProperty<Real> & _d2M_gdcgdcv;
  MaterialProperty<Real> & _Ms_g;

  MaterialProperty<Real> & _Efv;
  MaterialProperty<Real> & _Efg;

  MaterialProperty<Real> & _L_eta;
  MaterialProperty<Real> & _kb;
  MaterialProperty<Real> & _kappa_v;
  MaterialProperty<Real> & _kappa_g;
  MaterialProperty<Real> & _cv_eq;
  MaterialProperty<Real> & _W;
  MaterialProperty<Real> & _mug0;

  Real _int_width;
  Real _length_scale;
  Real _time_scale;
  Real _D0vac;
  Real _Emvac;
  Real _Efvac;
  Real _D0gas;
  Real _Emgas;
  Real _Efgas;
};

#endif //UO2VACGAS_H
