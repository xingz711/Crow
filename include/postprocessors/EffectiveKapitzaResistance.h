#ifndef EFFECTIVEKAPITZARESISTANCE_H
#define EFFECTIVEKAPITZARESISTANCE_H

#include "SideAverageValue.h"
#include "GeneralPostprocessor.h"

//Forward Declarations
class EffectiveKapitzaResistance;

template<>
InputParameters validParams<EffectiveKapitzaResistance>();

/**
 * Compute the thermal conductivity of the bulk.
 */
class EffectiveKapitzaResistance : public GeneralPostprocessor
{
public:
  EffectiveKapitzaResistance(const std::string & name, InputParameters parameters);

  virtual void initialize() {}
  virtual void execute() {}
  virtual Real getValue();

  Real _dx;
  Real _ko;
  Real _length_scale;

protected:
  const PostprocessorValue & _eff_cond;
};

#endif //EFFECTIVEKAPITZARESISTANCE_H
