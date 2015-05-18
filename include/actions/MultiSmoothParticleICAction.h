/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef POLYCRYSTALHEXGRAINICACTION_H
#define POLYCRYSTALHEXGRAINICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates ic for polycrystal hexagonal grain structure. Must have squared number of grains and periodic BCs.
 */
class MultiSmoothParticleICAction: public Action
{
public:
  MultiSmoothParticleICAction(const std::string & name, InputParameters params);

  virtual void act();

private:
  static const Real _abs_zero_tol;

  std::string _var_name_base;
  unsigned int _op_num;
  std::vector<Real> _x_positions;
  std::vector<Real> _y_positions;
  std::vector<Real> _z_positions;
  std::vector<Real> _input_radii;
  
  Real _int_width;
  bool _3D_spheres;
  unsigned int _num_dim;

  //unsigned int _numbub;
  //Real _bubspac;

  //unsigned int _numtries;
  //Real _radius;
  //Real _radius_variation;
  //MooseEnum _radius_variation_type;
  //bool _avoid_bounds;

};

template<>
InputParameters validParams<MultiSmoothParticleICAction>();

#endif //POLYCRYSTALHEXGRAINICACTION_H
