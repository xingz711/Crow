#ifndef CGTOTAUX_H
#define CGTOTAUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CgTOTAux;

template<>
InputParameters validParams<CgTOTAux>();

/**
 * Sum up two Variables into an AuxVariable
 * \todo This can be done more generally with vector coupling!
 * \todo This AuxKernel has no test coverage
 */
class CgTOTAux : public AuxKernel
{
public:
  CgTOTAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
};

#endif //CGTOTAUX_H
