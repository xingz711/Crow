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

#include "MultiBoundingBoxIC.h"

template<>
InputParameters validParams<MultiBoundingBoxIC>()
{
  InputParameters params = validParams<InitialCondition>();

  params.addRequiredParam<std::vector<Real> >("x1", "The x coordinate of the lower left-hand corner of the box");
  params.addRequiredParam<std::vector<Real> >("y1", "The y coordinate of the lower left-hand corner of the box");
  params.addParam<std::vector<Real> >("z1", "The z coordinate of the lower left-hand corner of the box");

  params.addRequiredParam<std::vector<Real> >("x2", "The x coordinate of the upper right-hand corner of the box");
  params.addRequiredParam<std::vector<Real> >("y2", "The y coordinate of the upper right-hand corner of the box");
  params.addParam<std::vector<Real> >("z2","The z coordinate of the upper right-hand corner of the box");

  params.addRequiredParam<std::vector<Real> >("inside", "The value of the variable inside the box");
  params.addParam<Real>("outside", 0.0, "The value of the variable outside the box");
  return params;
}

MultiBoundingBoxIC::MultiBoundingBoxIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _vx1(getParam<std::vector<Real> >("x1")),
    _vy1(getParam<std::vector<Real> >("y1")),
    _vz1(getParam<std::vector<Real> >("z1")),
    _vx2(getParam<std::vector<Real> >("x2")),
    _vy2(getParam<std::vector<Real> >("y2")),
    _vz2(getParam<std::vector<Real> >("z2")),
    _vinside(getParam<std::vector<Real> >("inside")),
    _outside(getParam<Real>("outside"))
{
  // Make sure inputs are the same length
  if (_vx1.size() != _vx2.size() || _vy1.size() != _vy2.size())
    mooseError("vector inputs must all be the same size");
  if (_vx1.size() != _vy1.size() || _vx2.size() != _vy2.size() || _vinside.size() != _vx1.size())
    mooseError("vector inputs must all be the same size");

  _nbox = _vx1.size();

  if (_vz1.size() != _nbox) // Zero out z vectors if 2D
  {
    _vz1.resize(_nbox);
    _vz2.resize(_nbox);
    for (unsigned int i = 0; i < _nbox; ++i)
    {
      _vz1[i] = 0.0;
      _vz2[i] = 0.0;
      /* _console << "x1 = " << _vx1[i] << " x2 = " << _vx2[i] << std::endl;
      _console << "y1 = " << _vy1[i] << " y2 = " << _vy2[i] << std::endl;
      _console << "z1 = " << _vz1[i] << " z2 = " << _vz2[i] << std::endl;*/
    }
  }
}

Real
MultiBoundingBoxIC::value(const Point & p)
{
  Real value = _outside;

  for (unsigned int b = 0; b < _nbox; ++b)
  {
    Point bottom_left = Point(_vx1[b], _vy1[b], _vz1[b]);
    Point top_right = Point(_vx2[b], _vy2[b], _vz2[b]);

    if (p(0) >= bottom_left(0) && p(0) <= top_right(0))
      if (p(1) >= bottom_left(1) && p(1) <= top_right(1))
      {
        if (_vz2[b] - _vz1[b] == 0.0)
          value = _vinside[b];
        else
          if (p(2) >= bottom_left(2) && p(2) <= top_right(2))
            value = _vinside[b];
      }
  }

  return value;
}
