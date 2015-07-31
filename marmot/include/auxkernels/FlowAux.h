#ifndef FLOWAUX_H
#define FLOWAUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class FlowAux;

template<>
InputParameters validParams<FlowAux>();

/**
 * Copy an element of a Real vector MaterialProperty into an AuxVariable
 * \todo This should have a more generic name
 */
class FlowAux : public AuxKernel
{
public:
  FlowAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();
  const MaterialProperty<std::vector<Real> > & _flow_vector;
  unsigned int _index;
};

#endif //FLOWAUX_H
