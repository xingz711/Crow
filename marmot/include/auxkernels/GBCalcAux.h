#ifndef GBCALCAUX_H
#define GBCALCAUX_H

#include "BndsCalcAux.h"

//Forward Declarations
class GBCalcAux;

template<>
InputParameters validParams<GBCalcAux>();
/**
 * This auxkernel differentiates GBs from the bulk and from bubbles and assigns a value of one to those areas.
 * It can be used to calcaulte the total GB area in the system by combining with the ElementIntegral postprocessor.
 * \todo the tests have an unused a min_c parameter
 */
class GBCalcAux : public BndsCalcAux
{
public:
  GBCalcAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

private:
  Real _GBthreshold;
};

#endif //GBCALCAUX_H
