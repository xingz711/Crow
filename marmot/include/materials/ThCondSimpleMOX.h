#ifndef THCONDSIMPLEMOX_H
#define THCONDSIMPLEMOX_H

#include "Material.h"
//#include "GrainTracker.h"
//#include "EBSDReader.h"

//Forward Declarations
class ThCondSimpleMOX;

template<>
InputParameters validParams<ThCondSimpleMOX>();

/**
 * This kernel computes the thermal conductivity of MOX fuel (with or without porosity).
 */
class ThCondSimpleMOX : public Material
{
public:
  ThCondSimpleMOX(const InputParameters & parameters);

protected:
  //virtual void initializeProperties();
  virtual void computeQpProperties();

private:

  Real _length_scale;
  Real _Gas_conductivity;
  Real _GB_conductivity;
  Real _Bulk_conductivity;
  Real _PPT_conductivity;

  unsigned int _n;
  std::vector<VariableValue *> _vals;

  MaterialProperty<Real> & _thermal_conductivity;

  MooseEnum _cube;

};


#endif //THCONDSIMPLEMOX_H
