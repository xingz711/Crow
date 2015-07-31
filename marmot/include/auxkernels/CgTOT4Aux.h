#ifndef CGTOT4AUX_H
#define CGTOT4AUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CgTOT4Aux;

template<>
InputParameters validParams<CgTOT4Aux>();

/**
 * Sum up two Variables into an AuxVariable
 * \todo This can be done more generally with vector coupling!
 * \todo This AuxKernel has no test coverage
 */
class CgTOT4Aux : public AuxKernel
{
public:
  CgTOT4Aux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
  VariableValue & _coupled_val3;
  VariableValue & _coupled_val4;

};

#endif //CGTOTAUX_H
