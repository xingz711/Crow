#ifndef CIRCLEGRAINVOIDIC_H
#define CIRCLEGRAINVOIDIC_H

#include "MultiSmoothCircleIC.h"
#include "MooseRandom.h"

// Forward Declarationsc
class CircleGrainVoidIC;

template<>
InputParameters validParams<CircleGrainVoidIC>();

/**
 * CircleGrainVoidIC initializes either grain or void values for a circle grain with voids distributed along the grain boundary.
 */
class CircleGrainVoidIC :
  public MultiSmoothCircleIC
{
public:
  CircleGrainVoidIC(const InputParameters & parameters);

private:

  MooseEnum _structure_type;

  unsigned int _op_num;
  unsigned int _op_index;

  bool _columnar_3D;

  Point _grain_center;
  Real _grain_radius;

  virtual void initialSetup();

  virtual void computeCircleCenters();

  virtual Real value(const Point & p);

  virtual RealGradient gradient(const Point & p);
};

#endif //CIRCLEGRAINVOIDIC_H
