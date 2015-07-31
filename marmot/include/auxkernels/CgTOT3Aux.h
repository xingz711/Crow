#ifndef CGTOT3AUX_H
#define CGTOT3AUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CgTOT3Aux;

template<>
InputParameters validParams<CgTOT3Aux>();

/**
 * Sum up two Variables into an AuxVariable
 * \todo This can be done more generally with vector coupling!
 * \todo This AuxKernel has no test coverage
 */
class CgTOT3Aux : public AuxKernel
{
public:
  CgTOT3Aux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
  VariableValue & _coupled_val3;

};

#endif //CGTOTAUX_H
