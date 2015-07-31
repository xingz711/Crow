#ifndef PERTURBEDCYLINDERIC_H
#define PERTURBEDCYLINDERIC_H

#include "InitialCondition.h"

// Forward Declarations
class PerturbedCylinderIC;

template<>
InputParameters validParams<PerturbedCylinderIC>();

/**
 * PerturbedCylinderIC creates a cylinder perturbed by a sinusoid to simulate Rayleigh instability
 * \f$ R(z) = R_0 + A\cos(kz)\f$.
 */
class PerturbedCylinderIC : public InitialCondition
{
public:
  PerturbedCylinderIC(const InputParameters & parameters);

  virtual Real value(const Point & p);
  virtual void initialSetup();

protected:
  MooseMesh & _mesh;
  MooseEnum _var_type;
  unsigned int _ncycl;

  Real _x1;
  Real _y1;
  Real _radius;
  Real _cyl_bottom;
  Real _cyl_top;
  Real _wi;
  Real _Amp;

  Point _bottom_left;
  Point _top_right;
  Point _range;
};

#endif //PERTURBEDCYLINDERIC_H
