#ifndef GRAINBOUNDARYBUBBLECOVERAGE_H
#define GRAINBOUNDARYBUBBLECOVERAGE_H

#include "SideAverageValue.h"
#include "GeneralPostprocessor.h"

//Forward Declarations
class GrainBoundaryBubbleCoverage;

template<>
InputParameters validParams<GrainBoundaryBubbleCoverage>();

/**
 * Compute the grain boundary coverage from the values from three other postprocessors:
 *   -the GB area
 *   -the porosity
 *   -the number of bubbles.
 * These postprocessors should be located above this one in the list.
 */
class GrainBoundaryBubbleCoverage : public GeneralPostprocessor
{
public:
  GrainBoundaryBubbleCoverage(const InputParameters & parameters);

  virtual void initialize() {}
  virtual void execute() {}
  virtual Real getValue();

  Real _Lx;
  Real _Ly;
  Real _GBwidth;

protected:
  const PostprocessorValue & _porosity;
  const PostprocessorValue & _numbub;
  const PostprocessorValue & _GBarea;
};

#endif //GRAINBOUNDARYBUBBLECOVERAGE_H
