#ifndef CALCULATEGASONGB_H
#define CALCULATEGASONGB_H

#include "GeneralPostprocessor.h"
#include "Transient.h"

//Forward Declarations
class CalculateGasOnGB;

template<>
InputParameters validParams<CalculateGasOnGB>();

/**
 * Compute the amount of fission gas on a grain boundary.
 */
class CalculateGasOnGB : public GeneralPostprocessor
{
public:
  CalculateGasOnGB(const InputParameters & parameters);

  virtual void initialize() {}
  virtual void execute() {}
  virtual Real getValue();

protected:
  Real _Pg;
  Real _radius;

  const PostprocessorValue & _gas_in_grain;
};

#endif //CALCULATEGASONGB_H
