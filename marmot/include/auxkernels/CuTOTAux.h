#ifndef CUTOTAUX_H
#define CUTOTAUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CuTOTAux;

template<>
InputParameters validParams<CuTOTAux>();

/**
 * Computes /f$ 1-c_1-c_2-2c_3-2c_4 /f$
 * \todo This AuxKernel has no test coverage
 */
class CuTOTAux : public AuxKernel
{
public:
  CuTOTAux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
  VariableValue & _coupled_val3;
  VariableValue & _coupled_val4;
};

#endif //CUTOTAUX_H
