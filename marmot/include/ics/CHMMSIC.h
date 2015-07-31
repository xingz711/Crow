#ifndef CHMMSIC_H
#define CHMMSIC_H

#include "InitialCondition.h"

// Forward Declarations
class CHMMSIC;

template<>
InputParameters validParams<CHMMSIC>();

/**
 * CHMMSIC creates the value and graident initial condition for the Cahn-Hilliard MMS solution
 */
class CHMMSIC : public InitialCondition
{
public:
  CHMMSIC(const InputParameters & parameters);

  virtual Real value(const Point & p);
  virtual RealGradient gradient(const Point & p);

private:
  Real _alpha;
};

#endif //CHMMSIC_H
