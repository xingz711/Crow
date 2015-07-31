/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef GENERICMATERIALMATERIAL_H
#define GENERICMATERIALMATERIAL_H

#include "Material.h"


//Forward Declarations
class GenericMaterialMaterial;

template<>
InputParameters validParams<GenericMaterialMaterial>();

/**
 * This material automatically declares that are copies of existing material
 * properties. This is a debugging material to facilitate easy exodiffing.
 */
class GenericMaterialMaterial : public Material
{
public:
  GenericMaterialMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  std::vector<MaterialPropertyName> _prop_new_names;
  std::vector<MaterialPropertyName> _prop_old_names;

  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _new_properties;
  std::vector<const MaterialProperty<Real> *> _old_properties;
};

#endif //GENERICMATERIALMATERIAL_H
