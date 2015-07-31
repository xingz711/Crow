#ifndef ONEBUBBLEGBC_H
#define ONEBUBBLEGBC_H

#include "GeneralPostprocessor.h"

//Forward Declarations
class OneBubbleGBC;

template<>
InputParameters validParams<OneBubbleGBC>();

/**
 * Compute the thermal conductivity of the bulk.
 * \todo This postprocessor has no test coverage
 */
class OneBubbleGBC : public GeneralPostprocessor
{
public:
  OneBubbleGBC(const InputParameters & parameters);

  virtual void initialize() {}
  virtual void execute() {}
  virtual Real getValue();

protected:
  Real _Ly;
  Real _Lz;
  Real _radius_bub;
};

#endif //ONEBUBBLEGBC_H
