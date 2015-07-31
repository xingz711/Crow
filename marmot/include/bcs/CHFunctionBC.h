#ifndef CHFUNCTIONBC_H
#define CHFUNCTIONBC_H

#include "IntegratedBC.h"


//Forward Declarations
class CHFunctionBC;
class Function;

template<>
InputParameters validParams<CHFunctionBC>();

/**
 * Function based generic integrated boundary condition.
 */
class CHFunctionBC : public IntegratedBC
{
public:
  CHFunctionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

  Function & _func;
};

#endif //CHFunctionBC_H
