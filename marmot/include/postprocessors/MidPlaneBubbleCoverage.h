#ifndef MIDPLANEBUBBLECOVERAGE_H
#define MIDPLANEBUBBLECOVERAGE_H

#include "ElementAverageValue.h"

//Forward Declarations
class MidPlaneBubbleCoverage;

template<>
InputParameters validParams<MidPlaneBubbleCoverage>();

/**
 * Compute the fraction of GB (bicrystal) covered with bubbles.
 */
class MidPlaneBubbleCoverage : public ElementAverageValue
{
public:
  MidPlaneBubbleCoverage(const InputParameters & parameters);

  virtual Real computeQpIntegral();

private:
  Real _GB_lengthy;
  Real _GB_lengthz;
  Real _Lx;
  Real _tol;
  unsigned int  _dim;
  Real _bub_threshold;
};

#endif //MIDPLANEBUBBLECOVERAGE_H
