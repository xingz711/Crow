#ifndef CONSERVEDUNIFORMVOIDSOURCE_H
#define CONSERVEDUNIFORMVOIDSOURCE_H

#include "ConservedNoiseBase.h"
#include "ConservedUniformVoidSourceVeneer.h"

//Forward delcarations
class ConservedUniformVoidSource;

template<>
InputParameters validParams<ConservedUniformVoidSource>()
{
  return validParams<ConservedNoiseBase>();
}

/**
 * Userobject that generates a uniformly distributed random number in the interval [0:1]
 * once per timestep for every quadrature point in a way that the integral
 * over all random numbers is zero.
 *
 * \see ConservedNoiseBase
 */
class ConservedUniformVoidSource : public ConservedUniformVoidSourceVeneer<ConservedNoiseBase>
{
public:
  ConservedUniformVoidSource(const std::string & name, InputParameters parameters) :
    ConservedUniformVoidSourceVeneer<ConservedNoiseBase>(name, parameters) {}
};

#endif //CONSERVEDUNIFORMVOIDSOURCE_H
