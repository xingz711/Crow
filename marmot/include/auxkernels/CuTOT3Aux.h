#ifndef CUTOT3AUX_H
#define CUTOT3AUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class CuTOT3Aux;

template<>
InputParameters validParams<CuTOT3Aux>();

/**
 * Computes /f$ 1-c_1-c_2-2c_3-2c_4-3*c_5 /f$
 * \todo This AuxKernel has no test coverage
 */
class CuTOT3Aux : public AuxKernel
{
public:
  CuTOT3Aux(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  VariableValue & _coupled_val1;
  VariableValue & _coupled_val2;
  VariableValue & _coupled_val3;
  VariableValue & _coupled_val4;
  VariableValue & _coupled_val5;

};

#endif //CUTOT3AUX_H
