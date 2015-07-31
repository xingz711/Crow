#ifndef FRACTUREACBULKMATRATE_H
#define FRACTUREACBULKMATRATE_H

#include "Material.h"
#include "Function.h"

class FractureACBulkMatRate;

template<>
InputParameters validParams<FractureACBulkMatRate>();

class FractureACBulkMatRate : public Material
{
public:
  FractureACBulkMatRate(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();
  virtual void initQpStatefulProperties();
  virtual void read_prop();

  MaterialProperty<Real> & _L;

  MaterialProperty<Real> &_gc_prop_tens;
  MaterialProperty<Real> &_gc_prop_tens_old;
  MaterialProperty<Real> &_gc_prop_comp;
  MaterialProperty<Real> &_gc_prop_comp_old;

  bool _has_function;
  Function * _function;

  Real _L0;

private:
  FileName _frac_prop_file_name;
};

#endif //FRACTUREACBULKMATRATE_H
