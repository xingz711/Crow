#ifndef ALGEBRADEBUG_H
#define ALGEBRADEBUG_H

#include "Kernel.h"

//Forward Declarations
class AlgebraDebug;

template<>
InputParameters validParams<AlgebraDebug>();

class AlgebraDebug : public Kernel
{
public:
  AlgebraDebug(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  VariableValue & _v;
  unsigned int _v_var;
};

#endif //ALGEBRADEBUG_H
