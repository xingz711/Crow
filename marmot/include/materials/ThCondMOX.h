#ifndef THCONDMOX_H
#define THCONDMOX_H

#include "Material.h"
//#include "GrainTracker.h"
#include "EBSDReader.h"

//Forward Declarations
class ThCondMOX;

template<>
InputParameters validParams<ThCondMOX>();

/**
 * This kernel computes the thermal conductivity of MOX fuel (with or without porosity).
 */
class ThCondMOX : public Material
{
public:
  ThCondMOX(const InputParameters & parameters);

protected:
  virtual void initializeProperties();
  virtual void computeProperties();
  const EBSDReader & _ebsd_reader;
  //const GrainTracker & _grain_tracker;

private:
  MaterialProperty<unsigned int> & _grn;
  //MaterialProperty<unsigned int> & _op;
  MaterialProperty<unsigned int> & _phase;

  unsigned int _op_num;
  bool _has_v;
  Real _length_scale;
  Real _Gas_conductivity;
  Real _GB_conductivity;
  Real _Bulk_conductivity;
  Real _PPT_conductivity;

  unsigned int _n;
  std::vector<VariableValue *> _vals;

  VariableValue & _eta;

  MaterialProperty<Real> & _thermal_conductivity;
};


#endif //THCONDMOX_H
