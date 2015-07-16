#ifndef SPLITCHWRESMTRXMOB_H
#define SPLITCHWRESMTRXMOB_H

#include "Kernel.h"

//Forward Declarations
class SplitCHWResMtrxMob;

template<>
InputParameters validParams<SplitCHWResMtrxMob>();

class SplitCHWResMtrxMob : public Kernel
{
public:
  SplitCHWResMtrxMob(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const MaterialProperty<RealTensorValue> & _mob;
  const MaterialProperty<RealTensorValue> & _dmobdc;
  unsigned int _c_var;
  VariableValue & _c;
};

#endif //SPLITCHWRESMTRXMOB_H
