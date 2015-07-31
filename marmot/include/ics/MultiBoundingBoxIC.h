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

#ifndef MULTIBOUNDINGBOXIC_H
#define MULTIBOUNDINGBOXIC_H

#include "InitialCondition.h"

// Forward Declarations
class MultiBoundingBoxIC;

template<>
InputParameters validParams<MultiBoundingBoxIC>();

/**
 * MultiBoundingBoxIC allows setting the initial condition of a value inside and outside of a specified box.
 * The box is aligned with the x,y,z axis... and is specified by passing in the x,y,z coordinates of the bottom
 * left point and the top right point. Each of the coordinates of the "bottom_left" point MUST be less than
 * those coordinates in the "top_right" point.
 *
 * When setting the initial condition if bottom_left <= Point <= top_right then the "inside" value is used.
 * Otherwise the "outside" value is used.
 */
class MultiBoundingBoxIC : public InitialCondition
{
public:
  MultiBoundingBoxIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

protected:
  Real _nbox;

  std::vector<Real> _vx1;
  std::vector<Real> _vy1;
  std::vector<Real> _vz1;

  std::vector<Real> _vx2;
  std::vector<Real> _vy2;
  std::vector<Real> _vz2;

  std::vector<Real> _vinside;
  Real _outside;
};

#endif //MULTIBOUNDINGBOXIC_H
