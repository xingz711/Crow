#ifndef TIMEDERIVATIVEAUX_H
#define TIMEDERIVATIVEAUX_H

#include "AuxKernel.h"
#include "TimeKernel.h"

//Forward Declarations
class TimeDerivativeAux;

template<>
InputParameters validParams<TimeDerivativeAux>();

/**
 * Store the time derivative of a Variable into an AuxVariable
 */
class TimeDerivativeAux : public AuxKernel
{
public:
  TimeDerivativeAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _u_dot;
};

#endif //TIMEDERIVATIVEAUX_H
