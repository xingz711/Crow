#ifndef VAAUX_H
#define VAAUX_H

#include "AuxKernel.h"
#include "Material.h"

//Forward Declarations
class VaAux;

template<>
InputParameters validParams<VaAux>();

/**
 * Calculate /f$ 1-c_1-c_2 /f$ for two coupled variables
 */
class VaAux : public AuxKernel
{
public:

  VaAux(const InputParameters & parameters);

protected:
    virtual Real computeValue();

    VariableValue & _coupled_val1;
    VariableValue & _coupled_val2;
};

#endif //VAAUX_H
