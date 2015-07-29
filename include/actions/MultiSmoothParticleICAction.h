/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef MULTISMOOTHPARTICLEICACTION_H
#define MULTISMOOTHPARTICLEICACTION_H

#include "InputParameters.h"
#include "Action.h"

/**
 * Automatically generates ic for polycrystal circular grain structure at specified locations and assign order parameters.
 */
class MultiSmoothParticleICAction: public Action
{
public:
  MultiSmoothParticleICAction(const  InputParameters params);

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

};

template<>
InputParameters validParams<MultiSmoothParticleICAction>();

#endif //MULTISMOOTHPARTICLEICACTION_H
