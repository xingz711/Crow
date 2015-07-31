#ifndef SINUSOIDALSURFACEIC_H
#define SINUSOIDALSURFACEIC_H

#include "InitialCondition.h"

// Forward Declarations
class SinusoidalSurfaceIC;

template<>
InputParameters validParams<SinusoidalSurfaceIC>();

/**
 * SinusoidalSurfaceIC creates a 1D sinusoidal surface in a 2D/3D domain.
 */
class SinusoidalSurfaceIC : public InitialCondition
{
public:
  SinusoidalSurfaceIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

  virtual void initialSetup();

protected:
  MooseMesh & _mesh;
  MooseEnum _dom_dim;
  MooseEnum _var_type;
  unsigned int _ncycl;

  Real _SH;
  Real _wi;
  Real _Amp;

  Point _bottom_left;
  Point _top_right;
  Point _range;
};

#endif //SINUSOIDALSURFACEIC_H
