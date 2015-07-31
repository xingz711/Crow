#ifndef CGBTOTAUX_H
#define CGBTOTAUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CgBTOTAux;

template<>
InputParameters validParams<CgBTOTAux>();

/**
 * Sum up three Variables into an AuxVariable
 * \todo This can be done more generally with vector coupling!
 * \todo This AuxKernel has no test coverage
 */
class CgBTOTAux : public AuxKernel
{
public:
  CgBTOTAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
  VariableValue & _coupled_val3;
};

#endif //CGBTOTAUX_H
