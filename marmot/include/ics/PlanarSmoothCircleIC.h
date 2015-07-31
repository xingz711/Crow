#ifndef PLANARSMOOTHCIRCLEIC_H
#define PLANARSMOOTHCIRCLEIC_H

#include "SmoothCircleBaseIC.h"

// Forward Declarations
class PlanarSmoothCircleIC;

template<>
InputParameters validParams<PlanarSmoothCircleIC>();

/**
 * PlanarsmoothCircleIC creates multiple SmoothCircles (number = numbub) that are randomly
 * positioned along a plane normal to either the x, y, or z directions.
 */
class PlanarSmoothCircleIC : public SmoothCircleBaseIC
{
public:
  PlanarSmoothCircleIC(const InputParameters & parameters);

  virtual void computeCircleRadii();

  virtual void computeCircleCenters();

protected:
  unsigned int _numbub;
  Real _bubspac;
  Real _position;
  MooseEnum _direction;

  unsigned int _numtries;
  Real _radius;
  Real _radius_variation;
  MooseEnum _radius_variation_type;
};

#endif //PLANARSMOOTHCIRCLEIC_H
