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

#include "GenericMaterialMaterial.h"

template<>
InputParameters validParams<GenericMaterialMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addParam<std::vector<MaterialPropertyName> >("prop_new_names", "The names of the new properties this material will have");
  params.addParam<std::vector<MaterialPropertyName> >("prop_old_names", "The names of the old properties the material will copy");
  return params;
}

GenericMaterialMaterial::GenericMaterialMaterial(const InputParameters & parameters) :
    Material(parameters),
    _prop_new_names(getParam<std::vector<MaterialPropertyName> >("prop_new_names")),
    _prop_old_names(getParam<std::vector<MaterialPropertyName> >("prop_old_names"))
{
  unsigned int num_new_names = _prop_new_names.size();
  unsigned int num_old_names = _prop_old_names.size();

  if (num_old_names != num_new_names)
    mooseError("Number of prop_old_names must match the number of prop_new_names for a GenericMaterialMaterial!");

  _num_props = num_old_names;

  _new_properties.resize(num_new_names);
  _old_properties.resize(num_old_names);

  for (unsigned int i=0; i<_num_props; i++)
  {
    _new_properties[i] = &declareProperty<Real>(_prop_new_names[i]);
    _old_properties[i] = &getMaterialPropertyByName<Real>(_prop_old_names[i]);
  }
}

void
GenericMaterialMaterial::computeQpProperties()
{
  for (unsigned int i=0; i<_num_props; i++)
    (*_new_properties[i])[_qp] = (*_old_properties[i])[_qp];
}
