/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef BICRYSTALICACTION_H
#define BICRYSTALICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates all variables to model a polycrystal with op_num orderparameters
 */
class BicrystalICAction: public Action
{
public:
  BicrystalICAction(const InputParameters & params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _op_num;

  Real _x1, _y1, _z1;
  Real _x2, _y2, _z2;
  Real _int;
};

template<>
InputParameters validParams<BicrystalICAction>();

#endif //BICRYSTALBOUNDINGBOXICACTION_H
